import os
import glob
import multiprocessing

SetOption('num_jobs', multiprocessing.cpu_count()) # build with all aviable cpu cores/threads

env = Environment(ENV=os.environ)

env.Append(CPPPATH = ["."])

conf = Configure(env)

env.Append(CXXFLAGS=['-march=native', '-mavx', '-maes', '-O3', '-std=c++14'])

env.Decider('MD5')
env.Program('example', ['example.cpp'])
