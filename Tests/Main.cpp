#include <memory>

#include <cpptest-suite.h>
#include <cpptest.h>

#include <Unit/Asset.h>

int main()
{
    Test::Suite unit;
    unit.add(std::auto_ptr< ::Test::Suite>( new Monocle::Test::Unit::Asset() ));
    
    Test::TextOutput output(Test::TextOutput::Verbose);
    unit.run(output, false);
}
