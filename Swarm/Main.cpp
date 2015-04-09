#include <Windows.h>
#include <memory>
#include <algorithm>
#include "Window.h"
#include "Renderer.h"
#include "World.h"

using namespace OnceMoreWithFeeling;
using namespace std;

const unsigned int NUM_BOIDS = 3072;
const float NOTICE_DISTANCE = 0.75f;
const float OPTIMAL_DISTANCE = 0.25f;
const float MAX_SPEED = 0.001f;
const float RESPONSIVENESS = 0.000002f;
const float WING_BEAT_SPEED = 0.003f;
const float FREAKOUT_CHANCE_PER_SECOND = 0.1f;

float boidVerts[] = {
    -0.2f, 0.0f, 0.4f,
    0.2f, 0.0f, 0.4f,
    0, 0, 0,
    0, 0, 0,
    -0.65f, 0.5f, -0.25f,
    0, 0, -0.5f,
    0, 0, 0,
    0.65f, 0.5f, -0.25f,
    0, 0, -0.5f
};

float cubeVerts[] = { 
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

struct Boid
{
    unsigned int id;
    Vector position;
    Vector velocity;
};

class KdTreeNode
{
public:
    KdTreeNode(vector<Boid> &boids, unsigned int depth);
    void WithinRange(const Vector centre, float distance, vector<unsigned int> &result);
    float DistanceSq(const Vector v) { return (value_.position - v).LengthSq(); }

private:
    Boid value_;
    unique_ptr<KdTreeNode> left_;
    unique_ptr<KdTreeNode> right_;
};

KdTreeNode::KdTreeNode(vector<Boid> &boids, unsigned int depth)
    : left_(nullptr), right_(nullptr)
{
    unsigned int median = boids.size() / 2;
    unsigned int axis = depth % 3;

    if (axis == 0)
        sort(boids.begin(), boids.end(), [](Boid a, Boid b){ return a.position.x < b.position.x; });
    else if (axis == 1)
        sort(boids.begin(), boids.end(), [](Boid a, Boid b){ return a.position.y < b.position.y; });
    else
        sort(boids.begin(), boids.end(), [](Boid a, Boid b){ return a.position.z < b.position.z; });

    value_ = boids[median];

    vector<Boid> leftPoints(boids.begin(), boids.begin() + median);
    if (leftPoints.size() > 0)
        left_ = make_unique<KdTreeNode>(leftPoints, depth + 1);

    vector<Boid> rightPoints(boids.begin() + median + 1, boids.end());
    if (rightPoints.size() > 0)
        right_ = make_unique<KdTreeNode>(rightPoints, depth + 1);
}

void KdTreeNode::WithinRange(const Vector centre, float distance, vector<unsigned int> &result)
{
    float dsq = DistanceSq(centre);
    if (dsq <= distance * distance)
    {
        result.push_back(value_.id);
        if (left_)
            left_->WithinRange(centre, distance, result);
        if (right_)
            right_->WithinRange(centre, distance, result);
    }
    else
    {
        float leftDistance = distance * 100;
        float rightDistance = distance * 100;
        if (left_)
            leftDistance = left_->DistanceSq(centre);
        if (right_)
            rightDistance = right_->DistanceSq(centre);
        if (leftDistance < rightDistance && left_)
            left_->WithinRange(centre, distance, result);
        else if (rightDistance < leftDistance && right_)
            right_->WithinRange(centre, distance, result);
    }
}

class SwarmWorld : public World
{
public:
    virtual void Init(shared_ptr<Renderer> renderer);
    virtual void Upate(float msecs);
    virtual void Draw(shared_ptr<Renderer> renderer);

private:
    vector<Boid> boids_;
    vector<float> anims_;
    float ermergerd_;
    shared_ptr<RenderObject> bird_;
    shared_ptr<RenderObject> skybox_;
    GLuint skyboxTexture_;
    Vector swarmCentre_;
};

void SwarmWorld::Init(shared_ptr<Renderer> renderer)
{
    renderer->AddShader("basic", "basic");
    renderer->AddShader("skybox", "skybox");

    shared_ptr<Buffer> boidVertexBuffer = make_shared<Buffer>();
    boidVertexBuffer->SetData(boidVerts, 27);

    shared_ptr<Object> o = make_shared<Object>();
    o->AttachBuffer(0, boidVertexBuffer);

    bird_ = make_shared<RenderObject>();
    bird_->object = o;
    bird_->program = "basic|basic";
    bird_->colour[0] = bird_->colour[1] = bird_->colour[2] = 0.f;

    for (unsigned int i = 0; i < NUM_BOIDS; ++i)
    {
        Boid b;
        b.id = i;
        b.position = Vector(RandF(-4, 4), RandF(-4, 4), RandF(-4, 4));
        b.velocity = Vector(RandF(-0.01f, 0.01f), RandF(-0.01f, 0.01f), RandF(-0.01f, 0.01f));

        boids_.push_back(b);
        anims_.push_back(RandF(0, 1));
    }

    ermergerd_ = 0;

    shared_ptr<Buffer> skyboxVertexBuffer = make_shared<Buffer>();
    skyboxVertexBuffer->SetData(cubeVerts, 108);
    shared_ptr<Object> skyboxObject = make_shared<Object>();
    skyboxObject->AttachBuffer(0, skyboxVertexBuffer);

    skybox_ = make_shared<RenderObject>();
    skybox_->object = skyboxObject;
    skybox_->program = "skybox|skybox";
    skybox_->textureBindings.insert(make_pair(0, "skybox"));

    vector<string> skyboxFilenames = { "posx.jpg", "negx.jpg", "posy.jpg", "negy.jpg", "posz.jpg", "negz.jpg" };
    renderer->AddCubeTexture("skybox", skyboxFilenames);

    renderer->SetCameraPosition(Vector(0, -3, 7));
    renderer->SetCameraLookAt(Vector(0, 0, 0));

    glDisable(GL_CULL_FACE);
}

void SwarmWorld::Upate(float msecs)
{
    // Freak out time!
    if (RandF(0, 1) < (FREAKOUT_CHANCE_PER_SECOND / 1000) * msecs)
        ermergerd_ = 250;

    swarmCentre_ = Vector();

    KdTreeNode kdTree(vector<Boid>(boids_.begin(), boids_.end()), 0);

    for (unsigned int i = 0; i < boids_.size(); ++i)
    {
        Boid current = boids_[i];

        vector<unsigned int> neighbourIds;
        kdTree.WithinRange(boids_[i].position, NOTICE_DISTANCE, neighbourIds);

        Vector cohesion;
        Vector seperation;
        Vector alignment;
        size_t woo = neighbourIds.size();
        for (auto n : neighbourIds)
        {
            if (current.id == n)
                continue;

            Boid b = boids_[n];
            Vector v = b.position - current.position;
            // cohesion and seperation
            if (v.LengthSq() > OPTIMAL_DISTANCE * OPTIMAL_DISTANCE)
                cohesion = cohesion + v;
            else
                seperation = seperation - v;
            // alignment
            alignment = alignment + b.velocity;
        }

        Vector cohesionPull = cohesion.Normalise() * 0.2f;
        Vector seperationPull = seperation.Normalise() * 1.5f;
        Vector alignmentPull = alignment.Normalise() * 0.5f;
        Vector centerPull = (current.position * -1) * 0.1f;

        if (ermergerd_ > 0)
            cohesionPull = cohesion * -4;

        Vector pull = cohesionPull + seperationPull + alignmentPull + centerPull;

        Vector acceleration = pull * RESPONSIVENESS;
        current.velocity = current.velocity + acceleration * msecs;

        if (current.velocity.LengthSq() > MAX_SPEED * MAX_SPEED)
            current.velocity = current.velocity.Normalise() * MAX_SPEED;
        current.position = current.position + current.velocity * msecs;
        
        boids_[i].position = current.position;
        boids_[i].velocity = current.velocity;

        swarmCentre_ = swarmCentre_ + current.position * (1.f /  boids_.size());

        anims_[i] += WING_BEAT_SPEED * msecs;
        if (anims_[i] > 1)
            anims_[i] -= 1;
    }

    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    ermergerd_ -= msecs;
}

void SwarmWorld::Draw(shared_ptr<Renderer> renderer)
{
    renderer->SetCameraLookAt(swarmCentre_);

    glDepthMask(GL_FALSE);
    renderer->Draw(skybox_);

    glDepthMask(GL_TRUE);
    for (int i = 0; i < NUM_BOIDS; ++i)
    {
        // Use our camera view transformation to align the 'mesh' to
        // its velocity vector
        Matrix align = Matrix::Camera(Vector(), boids_[i].velocity);
        bird_->transformation = Matrix::Translate(boids_[i].position) * align.Transpose() * Matrix::Scale(0.1f);

        bird_->colour[2] = anims_[i];
        renderer->Draw(bird_);
    }
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev, LPSTR commandLine, int show)
{
    srand(::GetTickCount());

    #ifdef OMWF_RECORDING
        OnceMoreWithFeeling::Window w(instance, show, 1280, 720);
    #else
        OnceMoreWithFeeling::Window w(instance, show);
    #endif
    shared_ptr<Renderer> renderer = make_shared<Renderer>();
    shared_ptr<World> world = make_shared<SwarmWorld>();

    return w.Loop(world, renderer);
}
