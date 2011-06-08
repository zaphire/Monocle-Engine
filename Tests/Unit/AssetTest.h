#pragma once

#include <cpptest/cpptest-suite.h>

#include <Asset.h>

namespace Monocle
{
    class Asset;
    
    namespace Test
    {
        namespace Unit
        {
            class DerivedAsset : public Monocle::Asset
            {
            public:
                DerivedAsset() : Monocle::Asset(Monocle::ASSET_UNDEFINED) {}
                
                void Unload() {}
                void Reload() {}
            };
            
            class Asset: public ::Test::Suite
            {
            public:
                Asset();
                
            protected:
                virtual void setup();
                virtual void tear_down();
                
            private:
                void Constructor();
                void Destructor();
                void ReferenceCount();
                void GetName();
                void GetExtension();
                
                Monocle::Asset *asset;
            };

        }
    }
}
