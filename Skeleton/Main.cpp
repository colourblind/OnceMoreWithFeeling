#include "Window.h"
#include "Renderer.h"
#include "World.h"

using namespace OnceMoreWithFeeling;
using namespace std;

class SkeletonWorld : public World
{
public:
    virtual void Init(shared_ptr<Renderer> renderer);
    virtual void Upate(float msecs);
    virtual void Draw(shared_ptr<Renderer> renderer);

private:

};

void SkeletonWorld::Init(shared_ptr<Renderer> renderer)
{

}

void SkeletonWorld::Upate(float msecs)
{

}

void SkeletonWorld::Draw(shared_ptr<Renderer> renderer)
{

}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev, LPSTR commandLine, int show)
{
    srand(::GetTickCount());

    OnceMoreWithFeeling::Window w(instance, show);
    shared_ptr<Renderer> renderer = make_shared<Renderer>();
    shared_ptr<World> world = make_shared<SkeletonWorld>();

    return w.Loop(world, renderer);
}
