#ifndef PORTAL_PGD0D86X
#define PORTAL_PGD0D86X

#include "LevelBlock.hpp"

// __________________
//|                  |
//|     -1     1     |
//|    1 +-----+     |
//|      |     |     |
//|      |     |     |
//|   -1 +-----+     |
//|__________________|

class Portal
{
    public:

        static Portal& loadFromFile(std::string filename);
        static Portal& loadFromName(std::string name);

        static bool namesLoaded;
        static void loadAllNames();
        
        // getter
        const std::string& getName() const;
        const LevelBlock& getGeometry() const;

    private:

        // constructor
        Portal(std::string filename);

        // content
        LevelBlock* geometry;
        std::string name;
};

#endif /* end of include guard: PORTAL_PGD0D86X */
