#include "Window.h"
#include "Renderer.h"
#include "World.h"
#include "Utils.h"

const float HEIGHT_SCALE = 16;
const unsigned int TERRAIN_SIZE = 96;

using namespace OnceMoreWithFeeling;
using namespace std;

vector<float> cubeVerts = {
    -0.5f, 0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, 0.5f,

    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,

    0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,

    -0.5f, -0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,

    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, 0.5f
};

class TerrainWorld : public World
{
public:
    virtual void Init(shared_ptr<Renderer> renderer);
    virtual void Upate(float msecs);
    virtual void Draw(shared_ptr<Renderer> renderer);

private:
    shared_ptr<RenderObject> skybox_;
    shared_ptr<RenderObject> terrain_;
    float rotation_ = 0;
    shared_ptr<Framebuffer> framebuffer_;
};

float GetHeight(vector<float> noise, int x, int y)
{
    x = x < 0 ? 0 : x >= TERRAIN_SIZE ? TERRAIN_SIZE - 1 : x;
    y = y < 0 ? 0 : y >= TERRAIN_SIZE ? TERRAIN_SIZE - 1: y;
    return noise[x * TERRAIN_SIZE + y] * HEIGHT_SCALE;
}

Vector GetNormal(vector<float> noise, int x, int y)
{
    Vector v0(0, GetHeight(noise, x, y), 0);
    Vector v1(-1, GetHeight(noise, x - 1, y), 0);
    Vector v2(0, GetHeight(noise, x, y - 1), -1);
    Vector v3(1, GetHeight(noise, x + 1, y), 0);
    Vector v4(0, GetHeight(noise, x, y + 1), 1);
    Vector v5(-1, GetHeight(noise, x - 1, y - 1), -1);
    Vector v6(1, GetHeight(noise, x + 1, y - 1), -1);
    Vector v7(1, GetHeight(noise, x + 1, y + 1), 1);
    Vector v8(-1, GetHeight(noise, x - 1, y + 1), 1);

    Vector v01 = v1 - v0;
    Vector v02 = v2 - v0;
    Vector v03 = v3 - v0;
    Vector v04 = v4 - v0;
    Vector v05 = v5 - v0;
    Vector v06 = v6 - v0;
    Vector v07 = v7 - v0;
    Vector v08 = v8 - v0;

    Vector v12 = Vector::Cross(v02, v01).Normalised();
    Vector v23 = Vector::Cross(v03, v02).Normalised();
    Vector v34 = Vector::Cross(v04, v03).Normalised();
    Vector v41 = Vector::Cross(v01, v04).Normalised();

    Vector v56 = Vector::Cross(v06, v05).Normalised();
    Vector v67 = Vector::Cross(v07, v06).Normalised();
    Vector v78 = Vector::Cross(v08, v07).Normalised();
    Vector v85 = Vector::Cross(v05, v08).Normalised();

    return (v12 + v23 + v34 + v41 + v56 + v67 + v78 + v85).Normalised();
}

void TerrainWorld::Init(shared_ptr<Renderer> renderer)
{
    renderer->AddShader("terrain", "terrain");
    renderer->AddShader("skybox", "skybox");
    renderer->AddShader("fbo", "fbo");

    vector<string> skyboxFilenames = { "posx.jpg", "negx.jpg", "posy.jpg", "negy.jpg", "posz.jpg", "negz.jpg" };
    renderer->AddCubeTexture("skybox", skyboxFilenames);

    renderer->AddTexture("grass.jpg");
    renderer->AddTexture("snow.jpg");
    renderer->AddTexture("rock.jpg");
    renderer->AddTexture("texture_mask.png");

    // Skybox
    shared_ptr<Buffer> skyboxVertexBuffer = make_shared<Buffer>();
    skyboxVertexBuffer->SetData(cubeVerts);
    shared_ptr<Object> skyboxObject = make_shared<Object>();
    skyboxObject->AttachBuffer(0, skyboxVertexBuffer);

    skybox_ = make_shared<RenderObject>();
    skybox_->object = skyboxObject;
    skybox_->program = "skybox|skybox";
    skybox_->textureBindings.insert(make_pair(0, "skybox"));

    // Create terrain
    vector<float> noise;
    CreateNoise(noise, 8, TERRAIN_SIZE, TERRAIN_SIZE, 8);

    vector<float> patchTexCoords = {
        0, 0,
        0, 1,
        1, 1,
        1, 1,
        1, 0,
        0, 0
    };
    vector<Vector> patchTexCoords2 = {
        Vector(0, 0, 0),
        Vector(0, 1, 0),
        Vector(1, 1, 0),
        Vector(1, 1, 0),
        Vector(1, 0, 0),
        Vector(0, 0, 0)
    };

    size_t numVerts = (TERRAIN_SIZE - 1) * (TERRAIN_SIZE - 1) * 6;
    vector<Vector> terrainVerts;
    vector<Vector> terrainNorms;
    vector<float> terrainTexCoords;
    vector<float> terrainTypes;
    vector<float> terrainTexCoords2;
    terrainVerts.reserve(numVerts);
    terrainNorms.reserve(numVerts);
    terrainTexCoords.reserve(numVerts * 2);
    terrainTypes.reserve(numVerts * 3);
    terrainTexCoords2.reserve(numVerts * 2);
    for (int i = 0; i < TERRAIN_SIZE - 1; ++i)
    {
        for (int j = 0; j < TERRAIN_SIZE - 1; ++j)
        {
            Matrix t = Matrix::Translate(i - (static_cast<float>(TERRAIN_SIZE) / 2), 0, j - (static_cast<float>(TERRAIN_SIZE) / 2));

            Vector v0 = t * Vector(-0.5f, GetHeight(noise, i, j), -0.5f);
            Vector v1 = t * Vector(-0.5f, GetHeight(noise, i, j + 1), 0.5f);
            Vector v2 = t * Vector(0.5f, GetHeight(noise, i + 1, j + 1), 0.5f);
            Vector v3 = t * Vector(0.5f, GetHeight(noise, i + 1, j), -0.5f);

            terrainVerts.push_back(v0);
            terrainVerts.push_back(v1);
            terrainVerts.push_back(v2);
            terrainVerts.push_back(v2);
            terrainVerts.push_back(v3);
            terrainVerts.push_back(v0);

            Vector n0 = GetNormal(noise, i, j);
            Vector n1 = GetNormal(noise, i, j + 1);
            Vector n2 = GetNormal(noise, i + 1, j + 1);
            Vector n3 = GetNormal(noise, i + 1, j);

            terrainNorms.push_back(n0);
            terrainNorms.push_back(n1);
            terrainNorms.push_back(n2);
            terrainNorms.push_back(n2);
            terrainNorms.push_back(n3);
            terrainNorms.push_back(n0);

            terrainTexCoords.insert(terrainTexCoords.end(), patchTexCoords.begin(), patchTexCoords.end());

            // This is supposed to mix the texture up a little so the repetition is less obvious.
            // Need a more regular texture with fewer features around the edge for this to work.
            // Leave it in for now.
            float rot = RandF(0, PI * 2);
            Matrix textureRotate = Matrix::Translate(0.5f, 0.5f, 0) * Matrix::Rotate(0, 0, rot) * Matrix::Translate(-0.5f, -0.5f, 0);
            for (auto t2 : patchTexCoords2)
            {
                Vector tn = textureRotate * t2;
                terrainTexCoords2.push_back(tn.x);
                terrainTexCoords2.push_back(tn.y);
            }
            
            float slope0 = min(1, powf(Vector::Dot(n0, Vector(0, 1, 0)) + 0.1f, 32));
            float slope1 = min(1, powf(Vector::Dot(n1, Vector(0, 1, 0)) + 0.1f, 32));
            float slope2 = min(1, powf(Vector::Dot(n2, Vector(0, 1, 0)) + 0.1f, 32));
            float slope3 = min(1, powf(Vector::Dot(n3, Vector(0, 1, 0)) + 0.1f, 32));
            float height0 = min(1, powf(v0.y / HEIGHT_SCALE + 0.35f, 32)) * slope0 * 2.f;
            float height1 = min(1, powf(v1.y / HEIGHT_SCALE + 0.35f, 32)) * slope1 * 2.f;
            float height2 = min(1, powf(v2.y / HEIGHT_SCALE + 0.35f, 32)) * slope2 * 2.f;
            float height3 = min(1, powf(v3.y / HEIGHT_SCALE + 0.35f, 32)) * slope3 * 2.f;

            vector<float> types = {
                slope0, 1.f - slope0, height0,
                slope1, 1.f - slope1, height1,
                slope2, 1.f - slope2, height2,
                slope2, 1.f - slope2, height2,
                slope3, 1.f - slope3, height3,
                slope0, 1.f - slope0, height0
            };
            terrainTypes.insert(terrainTypes.end(), types.begin(), types.end());
        }
    }

    auto vertexBuffer = make_shared<Buffer>();
    auto normalBuffer = make_shared<Buffer>();
    auto texCoordBuffer = make_shared<Buffer>();
    auto typeBuffer = make_shared<Buffer>();
    auto texCoord2Buffer = make_shared<Buffer>();
    vertexBuffer->SetData(terrainVerts);
    normalBuffer->SetData(terrainNorms);
    texCoordBuffer->SetData(terrainTexCoords);
    typeBuffer->SetData(terrainTypes);
    texCoord2Buffer->SetData(terrainTexCoords2);
    auto o = make_shared<Object>();
    o->AttachBuffer(0, vertexBuffer);
    o->AttachBuffer(1, normalBuffer);
    o->AttachBuffer(2, texCoordBuffer, 2);
    o->AttachBuffer(3, typeBuffer);
    o->AttachBuffer(4, texCoord2Buffer, 2);

    terrain_ = make_shared<RenderObject>();
    terrain_->object = o;
    terrain_->program = "terrain|terrain";
    terrain_->colour[0] = terrain_->colour[1] = terrain_->colour[2] = 1;

    terrain_->textureBindings.insert(make_pair(0, "grass.jpg"));
    terrain_->textureBindings.insert(make_pair(1, "snow.jpg"));
    terrain_->textureBindings.insert(make_pair(2, "rock.jpg"));
    terrain_->textureBindings.insert(make_pair(3, "texture_mask.png"));

    renderer->SetCameraPosition(Vector(0, 10, 0));

    unsigned int width, height;
    renderer->GetWindowSize(width, height);
    framebuffer_ = make_shared<Framebuffer>(width, height);

    renderer->AddTexture("fbo_colour", framebuffer_->GetTexture());
    renderer->AddTexture("fbo_depth", framebuffer_->GetDepth());
}

void TerrainWorld::Upate(float msecs)
{
    rotation_ += 0.00025f * msecs;
    if (rotation_ > PI * 2)
        rotation_ -= PI * 2;
}

void TerrainWorld::Draw(shared_ptr<Renderer> renderer)
{
    renderer->SetFramebuffer(framebuffer_);

    glClearColor(1, 0, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderer->SetCameraPosition(Vector(0, 10, 0));
    renderer->SetCameraLookAt(Vector(sinf(rotation_), 10, cosf(rotation_)));

    glDepthMask(GL_FALSE);
    renderer->Draw(skybox_);

    glDepthMask(GL_TRUE);
    renderer->Draw(terrain_);
    
    renderer->ResetFramebuffer();

    glClearColor(0, 0, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    unordered_map<unsigned int, string> textures;
    textures.insert(make_pair(0, "fbo_colour"));
    textures.insert(make_pair(1, "fbo_depth"));

    renderer->DrawFullscreenQuad("fbo|fbo", textures);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev, LPSTR commandLine, int show)
{
    srand(::GetTickCount());

    OnceMoreWithFeeling::Window w(instance, show);
    shared_ptr<Renderer> renderer = make_shared<Renderer>();
    shared_ptr<World> world = make_shared<TerrainWorld>();

    return w.Loop(world, renderer);
}
