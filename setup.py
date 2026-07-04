# Available at setup time due to pyproject.toml
from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup, Extension
#from setuptools.command.build_ext import build_ext
import sys

# 
# FOR /R . %f IN (include\*.h) DO git add "%f"
# 

__version__ = "1.1.11"

# The main interface is through Pybind11Extension.
# * You can add cxx_std=11/14/17, and then build_ext can be removed.
# * You can set include_pybind11=false to add the include directory yourself,
#   say from a submodule.
#
# Note:
#   Sort input source files if you glob sources to ensure bit-for-bit
#   reproducible builds (https://github.com/pybind/python_example/pull/53)

# 
# python3.14t.exe -m pip install H:\C++\AlexWeslowski\Divisors
# python.exe -m pip install git+https://github.com/AlexWeslowski/Divisors.git
# 
# import divisors as div
# div.divisors(33554430)
# div.divisors(134217726)
# 

STACK_SIZE = 134217728
STACK_SIZE = 268435456
STACK_SIZE = 536870912
extra_link_args = []
if sys.platform == "win32":
    extra_link_args.append(f"/STACK:{STACK_SIZE}")
else:
    extra_link_args.append(f"-Wl,-z,stack-size={STACK_SIZE}")

ext_modules = [
    #Extension(
    Pybind11Extension(
        "divisors",
        sources=["src/Globals.cpp", "src/ArrayArray.cpp", "src/AtomicBitset.cpp", "src/Combinations.cpp", "src/CombinationsIterator.cpp", "src/Point.cpp", "src/Divisors.cpp"],
        include_dirs=['src', 'include', 'include/pybind11', 'include/python', 'include/primesieve', 'include/boost'],
        library_dirs=["lib"],
        libraries=["primesieve"],
        define_macros=[("VERSION_INFO", __version__)],
        extra_compile_args=["-std=c++23"],
        extra_link_args=extra_link_args,
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
    python_requires=">=3.11",
)
