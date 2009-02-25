# -*- python -*-
#
# Setup our environment
#
import glob, os.path, re, os
import lsst.SConsUtils as scons

dependencies = ["python", "utils", "pex_exceptions"]

env = scons.makeEnv("sdqa",
                    r"$HeadURL: svn+ssh://svn.lsstcorp.org/DMS/sdqa/trunk/SConstruct $",
                    [["boost", "boost/shared_ptr.hpp"],
                     ["boost", "boost/regex.hpp", "boost_regex:C++"],
                     ["utils", "lsst/utils/Utils.h", "utils:C++"],
                     ["pex_exceptions", "lsst/pex/exceptions/Exception.h", "pex_exceptions:C++"],
                     ["python", "Python.h"], # needed for Swig
                    ])

env.Help("""
LSST Pipeline Execution SDQA package
""")

pkg = env["eups_product"]
env.libs[pkg] += env.getlibs(" ".join(dependencies))

#
# Build/install things
#
for d in Split("lib python/lsst/" + re.sub(r'_', "/", pkg) + " tests doc"):
    SConscript(os.path.join(d, "SConscript"))

env['IgnoreFiles'] = r"(~$|\.pyc$|^\.svn$|\.o$)"

Alias("install", [env.Install(env['prefix'], "python"),
                  env.Install(env['prefix'], "include"),
                  env.InstallAs(os.path.join(env['prefix'], "doc", "doxygen"),
                                os.path.join("doc", "htmlDir")),
                  env.InstallEups(env['prefix'] + "/ups", glob.glob("ups/*.table"))])

scons.CleanTree(r"*~ core *.so *.os *.o *.pyc")

#
# Build TAGS files
#
files = scons.filesToTag()
if files:
    env.Command("TAGS", files, "etags -o $TARGET $SOURCES")

env.Declare()

