#include <cpptest/cpptest-assert.h>

#include <Asset.h>
#include "AssetTest.h"

namespace Monocle
{
    namespace Test
    {
        namespace Unit
        {
            Asset::Asset()
            {
                TEST_ADD(Asset::Constructor)
                TEST_ADD(Asset::ReferenceCount)
                //TEST_ADD(Asset::GetName)
                //TEST_ADD(Asset::GetExtension)
            }
            
            void Asset::setup()
            {
                asset = new DerivedAsset();
            }
            
            void Asset::tear_down()
            {
                delete asset;
                asset = NULL;
            }
            
            void Asset::Constructor()
            {
                TEST_ASSERT(asset->type == Monocle::ASSET_UNDEFINED);
                TEST_ASSERT(asset->referenceCount == 0);
                TEST_ASSERT(asset->filename == "");
            }
            
            void Asset::ReferenceCount()
            {
                TEST_ASSERT(asset->referenceCount == 0);
                asset->AddReference();
                TEST_ASSERT(asset->referenceCount == 1);
                asset->AddReference();
                TEST_ASSERT(asset->referenceCount == 2);
                asset->RemoveReference();
                TEST_ASSERT(asset->referenceCount == 1);
            }
            
            void Asset::GetName()
            {
                std::string name = asset->GetName();
                TEST_FAIL(name.data());
            }
            
            void Asset::GetExtension()
            {
                std::string ext = asset->GetExtension();
                TEST_FAIL(ext.data());
            }
        }
    }
}
