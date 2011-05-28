#include <memory>

#include <cpptest-suite.h>
#include <cpptest.h>

#include <Unit/AssetTest.h>
#include <Unit/FontAssetTest.h>
#include <Unit/AssetsTest.h>

int main()
{
    Test::Suite unit;
    unit.add(std::auto_ptr< ::Test::Suite>( new Monocle::Test::Unit::Assets()));
    unit.add(std::auto_ptr< ::Test::Suite>( new Monocle::Test::Unit::Asset() ));
    unit.add(std::auto_ptr< ::Test::Suite>( new Monocle::Test::Unit::FontAsset() ));
    
    Test::TextOutput output(Test::TextOutput::Verbose);
    unit.run(output, false);
}
