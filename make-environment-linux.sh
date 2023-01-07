#!/bin/bash

build_iwyu() {
    # Some distributions (e.g. centos8, oracel linux 8) don't have iwyu 
    # in their repos so we build it from the source. 
    git clone https://github.com/include-what-you-use/include-what-you-use.git
    cd include-what-you-use
    if [[ `command -v clang` ]]; then
        local clang_version=`clang --version | grep version | cut -d' ' -f3 | cut -d'.' -f1`
        git checkout clang_${clang_version} || die "clang_${clang_version} branch not found."
        cd ..
        mkdir build && cd build
        if [[ "${clang_version}" -gt 6 ]]; then
            cmake -G "Unix Makefiles" -DCMAKE_PREFIX_PATH=/usr/lib/llvm-7 ../include-what-you-use
        else
            cmake -G "Unix Makefiles" -DIWYU_LLVM_ROOT_PATH=/usr/lib/llvm-6.0 ../include-what-you-use
        fi
        test -f Makefile && make
    fi
}

get_lcov_if_not_available() {
    # lcov is currently not available on some repo distribution
    # such as Oracle linux 9. Therefore, we first check if it is available 
    # in the distribtion's repo, otherwise download it from git repo
    # and install it locally.
    (${1} list available | grep lcov) && return 0
    local rpm_url=$(curl -s https://api.github.com/repos/linux-test-project/lcov/releases | \
            grep browser_download_url | \
            grep 'noarch.rpm' | \
            head -n 1 | \
            cut -d'"' -f 4);
    local rpm_name=$(echo ${rpm_url} | rev| cut -d'/' -f 1 | rev)
    wget -O ${rpm_name} ${rpm_url}
    ${1} localinstall -y ${rpm_name}
}

download_and_install_epel() {
    # we have reached here therefore it must be a rehdat 7 based distribution.
    wget http://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm
    rpm -Uvh epel-release-latest-7*.rpm
}

enable_additional_repos_on_redhat() {
    local repo_name="powertools"
    ${1} install -y epel-release || download_and_install_epel
    local dist_name=$(cat /etc/os-release | grep '^NAME')
    [[ "${dist_name}" == *"Oracle"* ]] && repo_name="ol*_codeready_builder"
    if [[ "${1}" == "dnf" ]]; then
        ${1} config-manager --set-enabled ${repo_name}
    elif [[ "${1}" == "yum" ]]; then
        yum-config-manager --enable ${repo_name}
    fi
}

make_env_on_archlinux() {
    pacman -Sy base-devel \
                cmake \
                llvm \
                clang \
                lcov \
                cppcheck \
                valgrind \
                git \
                --noconfirm
    build_iwyu
}

make_env_on_redhat() {
    local packages=(llvm llvm-devel clang clang-* cmake lcov cppcheck valgrind)
    if [[ "${1}" == "yum" ]]; then
        ${1} groupinstall -y "Development Tools"
        enable_additional_repos_on_redhat ${1}
        packages=(iwyu ${packages[@]})
        ${1} install -y ${packages[@]}
    elif [[ "${1}" == "dnf" ]]; then
        ${1} group install -y "Development Tools"
        enable_additional_repos_on_redhat ${1}
        get_lcov_if_not_available ${1}
        ${1} install -y ${packages[@]}
        build_iwyu
    fi
}

make_env_on_debian() {
    apt install -y build-essential \
                    iwyu \
                    llvm \
                    clang-* \
                    cmake \
                    lcov \
                    cppcheck \
                    valgrind
}

main() {
    local LINUX_PACKAGE_MANAGER=''
    # We can add more package managers for more distribution support
    # e.g. can add zypper for opensuse. 
    declare -a pack_mans=("dnf" "yum" "apt" "pacman") 
    for pack_man in ${pack_mans[@]}; do 
        if [[ `command -v ${pack_man}` ]]; then 
            LINUX_PACKAGE_MANAGER=${pack_man} 
            echo "Package manager found: ${pack_man}" 
            break 
        fi 
    done 
    if [[ -n "${LINUX_PACKAGE_MANAGER}" ]]; then 
        case "${LINUX_PACKAGE_MANAGER}" in
            "yum") make_env_on_redhat ${LINUX_PACKAGE_MANAGER};;
            "dnf") make_env_on_redhat ${LINUX_PACKAGE_MANAGER};;
            "apt") make_env_on_debian ;;
            "pacman") make_env_on_archlinux ;;
            *) ;;
        esac 
    else 
        die
    fi 
}

die() {
    local die_msg="This Linux distribution is not supported."
    [[ -z "${1}" ]] && echo ${die_msg} || echo ${1}
    false
    exit
}

main
