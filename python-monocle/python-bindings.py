import os
from pyplusplus import module_builder

#Creating an instance of class that will help you to expose your declarations
mb = module_builder.module_builder_t( [r"../Code/Monocle.h"]
                                      , gccxml_path=r"C:/path/to/gcc_xml.exe" 
                                      , working_directory=r"../Code"
                                      , include_paths=[u'../Code/OpenAL',u'../Code/Graphics',u'../Code/Colliders',u'../Code/Windows', u'../Code/OpenGL', '../Code']
                                      , define_symbols=['MONOCLE_WINDOW', 'MONOCLE_OPENGL'] )


#Well, don't you want to see what is going on?
#mb.print_declarations()

#Creating code creator. After this step you should not modify/customize declarations.
mb.build_code_creator( module_name='Monocle' )

#Writing code to file.
mb.write_module( './bindings.cpp' )
