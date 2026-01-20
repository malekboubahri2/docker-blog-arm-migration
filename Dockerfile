FROM centos:6

# CentOS 6 reached EOL, need to use vault mirrors
RUN sed -i 's|^mirrorlist=|#mirrorlist=|g' /etc/yum.repos.d/CentOS-Base.repo && \
    sed -i 's|^#baseurl=http://mirror.centos.org|baseurl=http://vault.centos.org|g' /etc/yum.repos.d/CentOS-Base.repo

# Install EPEL repository (required for some development tools)
RUN yum install -y epel-release && \
    sed -i 's|^mirrorlist=|#mirrorlist=|g' /etc/yum.repos.d/epel.repo && \
    sed -i 's|^#baseurl=http://download.fedoraproject.org/pub/epel|baseurl=http://archives.fedoraproject.org/pub/archive/epel|g' /etc/yum.repos.d/epel.repo

# Install Developer Toolset 2 for better C++11 support (GCC 4.8)
RUN yum install -y centos-release-scl && \
    sed -i 's|^mirrorlist=|#mirrorlist=|g' /etc/yum.repos.d/CentOS-SCLo-scl.repo && \
    sed -i 's|^mirrorlist=|#mirrorlist=|g' /etc/yum.repos.d/CentOS-SCLo-scl-rh.repo && \
    sed -i 's|^# baseurl=http://mirror.centos.org|baseurl=http://vault.centos.org|g' /etc/yum.repos.d/CentOS-SCLo-scl.repo && \
    sed -i 's|^# baseurl=http://mirror.centos.org|baseurl=http://vault.centos.org|g' /etc/yum.repos.d/CentOS-SCLo-scl-rh.repo

# Install build tools
RUN yum install -y \
    devtoolset-2-gcc \
    devtoolset-2-gcc-c++ \
    devtoolset-2-binutils \
    make \
    && yum clean all

WORKDIR /app
COPY *.h *.cpp ./

# AVX2 intrinsics are used in the code
RUN scl enable devtoolset-2 "g++ -O2 -mavx2 -o benchmark \
    main.cpp \
    matrix_operations.cpp \
    -std=c++11"

CMD ["./benchmark"]