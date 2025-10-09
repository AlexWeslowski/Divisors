# Available at setup time due to pyproject.toml
from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup, Extension
#from setuptools.command.build_ext import build_ext

__version__ = "1.0.2"

# The main interface is through Pybind11Extension.
# * You can add cxx_std=11/14/17, and then build_ext can be removed.
# * You can set include_pybind11=false to add the include directory yourself,
#   say from a submodule.
#
# Note:
#   Sort input source files if you glob sources to ensure bit-for-bit
#   reproducible builds (https://github.com/pybind/python_example/pull/53)

# 
# Scripts\pip.exe install C:\Users\alex.weslowski\Documents\C++\Divisors
# 


ext_modules = [
    #Extension(
    Pybind11Extension(
        "divisors",
        sources=["src/Globals.cpp", "src/ArrayArray.cpp", "src/Combinations.cpp", "src/CombinationsIterator.cpp", "src/Point.cpp", "src/Divisors.cpp"],
        include_dirs=['src', 'include', 'include/pybind11', 'include/python', 'include/primesieve'],
        library_dirs=["lib"],
        libraries=["primesieve"],
        define_macros=[("VERSION_INFO", __version__)],
        #extra_compile_args=["-std=c++17"],
    ),
]

setup(
    name="divisors",
    version=__version__,
    author="Alex Weslowski",
    author_email="ikswolsewa@gmail.com",
    url="https://github.com/AlexWeslowski",
    description="Divisors",
    long_description="",
    ext_modules=ext_modules,
    extras_require={},
    # Currently, build_ext only provides an optional "highest supported C++
    # level" feature, but in the future it may provide more features.
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.9",
)