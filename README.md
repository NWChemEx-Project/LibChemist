# LibChemist

Molecule and Basis Set Tools for NWChemEx

Building LibChemist
-------------------

LibChemist is built using 
[CPP](https://github.com/CMakePackagingProject/CMakePackagingProject.git).
Assuming you have already installed CPP, that you are on a sane Unix-like 
computer, and you are willing to let LibChemist build all dependencies, then 
the following will suffice to build LibChemist:

```
git clone https://github.com/NWChemEx-Project/SDE.git
cd SDE
cmake -H. -Bbuild -DCPP_GITHUB_TOKEN=<your super-secret token> \
                  -DMPI_ROOT=<path/to/your/mpi/installation> \
                  -DCMAKE_PREFIX_PATH=<where/you/installed/CPP> \                  
                  -DCMAKE_INSTALL_PREFIX=<where/you/want/to/install/LibChemist>
cd build
cmake --build .
#May need to run as an admin depending on where you are installing
cmake --build . --target install  
```
The build process is not capable of building MPI so you will have to provide a
path to a known installation via the `MPI_ROOT` variable. The GitHub token is
necessary because, at the moment, TAMM, SDE, and Utilities are private 
repositories (instructions for generating a token are 
[here](https://help.github.com/articles/creating-a-personal-access-token-for-the-command-line/)).

For finer-grained control over the build we direct the reader to the more 
thorough CPP build instructions located 
[here](https://cmakepackagingproject.readthedocs.io/en/latest/end_user/quick_start.html)

and note that LibChemist depends on several other projects:

- [utilities](https://github.com/NWChemEx-Project/Utilities)
- [SDE](https://github.com/NWChemEx-Project/SDE)
  - [bphash](https://github.com/bennybp/BPHash)
  - [pybind11](https://github.com/pybind/pybind11)
    - Requires a development version of Python
  - [cereal](https://github.com/USCiLab/cereal)
- [TAMM](https://github.com/NWChemEx-Project/TAMM)
  - [HPTT](https://github.com/ajaypanyala/hptt)
  - [MSGSL](https://github.com/Microsoft/GSL)
  - [LAPACKE](http://www.netlib.org/lapack/)
  - [Eigen3](https://github.com/eigenteam/eigen-git-mirror)
  - [GlobalArrays](https://github.com/GlobalArrays/ga)
  - MPI   
- (For testing only)[Catch2](https://github.com/catchorg/Catch2)
