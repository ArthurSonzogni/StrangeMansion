#include "utils/Singleton.hpp"
#include <string>

class GLFWwindow;

class Application
{
    public:

    Application();

    static Application& getInstance();

    // get the window id
    GLFWwindow* getWindow() const;

    // window control
    void exit();

    // delta time between frame and time from beginning
    float getFrameDeltaTime() const;
    float getTime() const;

    // application run
    void run();

    // Application informations
    int getWidth();
    int getHeight();

    private:

	Application& operator=(const Application&) {return *this;}

    GLFWwindow* window;
    float time;
    float deltaTime;

    enum State {
        stateReady,
        stateRun,
        stateExit
    };

    State state;

    protected:

	Application(const Application&) {};

    std::string title;
    int width;
    int height;

    virtual void loop();

};
