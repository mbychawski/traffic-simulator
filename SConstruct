# skrypt dla SCons wykorzystujacy boost_unit_test_framework linkowana dynamicznie
# nalezy ustawic odpowiednio sciezki do biblioteki boost
import platform, os

sources = ['src/test.cpp', './src/MapFactory.cpp', './src/Cross.cpp', './src/Spawn.cpp', './src/Road.cpp', './src/CrossRules.cpp', './src/ShortestPath.cpp' ]#, './src/VehicleCreator.cpp']#, './src/Scene.cpp']

qt_dir = '{QT-PATH}'
boost_dir = '{BOOST-PATH}'

qt_modules = ['QtCore', 'QtGui', 'QtWidgets']
libs = ['Qt5Core', 'Qt5Gui', 'Qt5Widgets']

include = ['include']
libpath = ['lib']

qtEnv = Environment(ENV = os.environ, TARGET_ARCH = 'x86')
qtEnv['QT5DIR'] = qt_dir
qtEnv.Tool('qt5')
qtEnv.EnableQt5Modules(qt_modules)

include.append(qt_dir + '/include')
libpath.append(qt_dir + '/lib')
libpath.append('.')

for i, val in enumerate(qt_modules):
    include.append(qt_dir + '/include/' + qt_modules[i])

include.append( boost_dir )
libpath.append( boost_dir + '/stage/lib' )
#libs.append('boost_unit_test_framework')


env = qtEnv.Clone()
#ustawienia kompilatora visual studio 2010 (cl)
if(platform.system() == "Windows"):
    env.Append( CPPFLAGS = ' /EHsc /MD /D "WIN32" /D "_CONSOLE" /W4 /DQT_NO_KEYWORDS' )

else:
    print platform.system() + " not supported"


env_test = env.Clone()

if(platform.system() == "Linux"):
    libs.append('boost_unit_test_framework')
elif(platform.system() == "Windows"):
    pass

env.Append( CPPPATH = include)
env.Append( LIBPATH = libpath)
env.Append( BINPATH = 'bin')

env.Uic5(Glob('*.ui'))
env.Program(target = 'tests', source = sources, LIBS = libs)
#env.Program( source = sources, target = 'test' )



