import os

u_target = 'midden'

u_root_dir = '.'
u_public_dir = u_root_dir + '/public'
u_include_dir = u_root_dir + '/include'
u_install_dir = u_root_dir + '/release/bin'
u_source_dir = u_root_dir + '/src'


u_ccflags = ['-Wall', '-g3', '-std=c++0x']
u_cpppath = []
u_cpppath.append(u_include_dir)

u_cpppath.append(u_public_dir)
u_cpppath.append(u_public_dir + "/base")
u_cpppath.append(u_public_dir + "/common")
u_cpppath.append(u_public_dir + "/log")
u_cpppath.append(u_public_dir + "/net")
u_cpppath.append(u_public_dir + "/http")
u_cpppath.append(u_public_dir + "/thread")


u_cpppath.append(u_source_dir)

u_libpath = ['/usr/local/lib', '/usr/local/lib64', '/usr/lib64']

u_libs = ['pthread', 'rt']

u_cppfiles =  Glob(u_public_dir + '/common/*.cpp') + \
    Glob(u_public_dir + '/log/*.cpp') + \
    Glob(u_public_dir + '/net/*.cpp') + \
    Glob(u_public_dir + '/http/*.cpp') + \
    Glob(u_public_dir + '/thread/*.cpp') + \
    Glob(u_source_dir + '/*.cpp')


env = Environment()

env.Append(CCFLAGS = u_ccflags)
env.Append(CPPPATH = u_cpppath)
env.Append(LIBPATH = u_libpath)
env.Append(LIBS = u_libs)

psexe = env.Program(u_target, u_cppfiles)

output = os.popen("mkdir -p " + u_install_dir).read()
env.Alias('install', env.Install(u_install_dir, psexe))
