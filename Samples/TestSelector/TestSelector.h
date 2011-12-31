//
//  TestSelector.h
//  MonocleIOS
//
//  Created by Josh Whelchel on 7/28/11.
//  Copyright 2011 jwmusic.org. All rights reserved.
//

#ifndef MonocleIOS_TestSelector_h
#define MonocleIOS_TestSelector_h

#include "Scene.h"

using namespace Monocle;

namespace TestSelector
{
    class GameScene : public Scene
    {
    public:
        
        GameScene();
        
        void Begin();
        void Update();
    };
};

#endif
