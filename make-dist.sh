#!/bin/sh

#
# Creates a new Distribution
# see ./make-dist.sh -h for help
#

# To Do
#
# * test
# * getting the documentation really sucks! Check if there is a better way 
# * auto commit Change-Log (?)
# * upload on sf.net
#

if [ -e .package-defaults.sh ]; then
   . ./.package-defaults.sh
fi

if [ -z "$COOKIES" ]; then
    COOKIES=
fi

if [ -z "$PACKAGE" ]; then
    PACKAGE=fragments
fi

if [ -z "$TMPDIR" ]; then
# you should rather redirect it to ~/tmp or any other non glob-writeable
# directory!
    TMPDIR=/tmp
fi

if [ -e VERSION ]; then
    . ./VERSION
fi

help () {
    echo "usage: $0 -hgsi -c <cookie-file> -p <package name> -v <version>"
    echo -e "\t-c\tSet to Cookie-File for wget (e.g. check ~/.firefox/Profiles/.../cookies.txt) [Default: $COOKIES]"
    echo -e "\t-p\tSet Package Name [Default: $PACKAGE]"
    echo -e "\t-v\tSet Version [Default: $VERSION]"
    if [ "$SCM" == "git" -o "$SCM" == "svn" -o "$SCM" == "off" ]; then
        SCMDEFAULT="$SCM"
    elif [ -z "$SCM" ]; then
        SCMDEFAULT="autodetect"
    else
        SCMDEFAULT="unkown (ERROR)"
    fi
    echo -e "\t-s\tUse Subversion for ChangeLog [Default: $SCMDEFAULT]"
    echo -e "\t-g\tUse git for ChangeLog [Default: $SCMDEFAULT]"
    echo -e "\t-k\tNo Source Code Management tool [Defailt: $SCMDEFAULT]"
    echo -e "\t-i\tJust print Info and exit"
    echo -e "\t-n\tDeactivate parts of the tasks"
    echo
    echo -e "The following Tasks are performed: "
    echo -e "\t* create change log"
    echo -e "\t* run tests"
    echo -e "\t* clean (make dist-clean, etc)"
    echo -e "\t* check for important files (VERSION, etc)"
    echo -e "\t* download documentation from SF"
    echo -e "\t* compose Changelog from git or subversion"
    echo -e "\t* abstract Changelog"
    echo -e "\t* create a final package (.tar.bz2)"
    if [ \! -e .package-defaults.sh ]; then
        echo
        echo -e "You can set default variables in .package-defaults.sh"
    fi
}

info () {
    echo "Dist Info"
    echo -e "\tPackage Name: $PACKAGE Version: $VERSION"
    echo -e "\tOutput Package: ${PACKAGE}-${VERSION}.tar.bz2"
    echo -e "\tSCM: ${SCM}"
#    ...
}

args=`getopt hisgc:v:n: $*`
if [ $? != 0 ]; then
    help
    exit 1
fi
set -- $args
for i; do
    case "$i" in
        -h)
            help
            exit 1
            shift;;
        -c)
            COOKIES="$2"
            shift; shift;;
        -p)
            PACKAGE="$2"
            shift; shift;;
        -v)
            VERSION="$2"
            shift; shift;;
        -g)
            SCM="git"
            shift;;
        -s)
            SCM="svn"
            shift;;
        -k)
            SCM="off"
            shift;;
        -i)
            ONLYINFO=true
            shift;;
        -n)
            echo "$2" | grep "tests" -
            NO_TESTS=$?
            echo "$2" | grep "doc" -
            NO_DOC=$?
            echo "$2" | grep "changelog" -
            NO_CHANGE_LOG=$?
            #TODO ...
            shift; shift;;
    esac
done

download_doc () {
    #
    # At the moment you need Dev-Rights on SF to download the HTMLized version
    # of the wiki. Thus wget needs an existing session on SF in a Cookie-File.
    # See man wget for help about the layout of the Cookie-File and where to
    # find it.
    #

    echo "Downloading Documentation"
    mkdir -p "$DOCDIR"
    wget --load-cookies "$COOKIES" -O "$DOCDIR/doc.tgz" 'http://fragments.wiki.sourceforge.net/space/backup/tgz/html' > /dev/null
    if [ $? != 0 ]; then
        echo "ERROR: Could not download the documentation from SourceForge"
        echo "\t(Please check if you have a cookie for an existing session)"
        exit 1
    fi
    OLD=`pwd`
    cd "$DOCDIR"
    tar xzf "doc.tgz"
    rm -f "doc.tgz"
    cd "$OLD"
}

create_package_dir () {
    echo "Create Package Dir: ${TMPPKGDIR}"

    mkdir -p "$TMPPKGDIR"
    cp -r . "$TMPPKGDIR"
    cd "$TMPPKGDIR"
    if [ "$?" != "0" ]; then
        echo "cd $TMPPKGDIR failed"
        exit 1
    fi
}

remove_package_dir () {
    echo "Removing Package Dir: ${TMPPKGDIR}"
    cd "$ORIGPATH"
    rm -rf "$TMPPKGDIR"
}

run_tests () {
#    echo "Running Tests"
    make tests
    if [ "$?" != "0" ]; then
        echo "Tests failed! Can't create package with failing tests! (See option '-n tests' to change this behaviour)"
        exit 1
    fi
}

clean () {
    echo "Cleaning"
    make dist-clean
    find . -name '*~' -exec rm {} \; # remove temporary files
    rm -rf .svn make-dist.sh
}

set_version_file () {
    echo "Setting VERSION"
    echo "VERSION=${VERSION}" > VERSION
}

create_package () {
    echo "Creating Package: ${PKGFILE}"
    tar cjf "${PKGFILE}" "${TMPPKGDIR}"
}

autodetect_scm () {
    if ls .svn &> /dev/null; then
        SCM="svn"
    elif git-show &> /dev/null; then
        SCM="git"
    else
        echo "Rep not managed by (known) SCM."
        SCM="off"
    fi
}

scm_change_log () {
    echo "from SCM"
    if [ "$SCM" == "svn" ]; then
        echo "Created ChangeLog from SVN" > DetailedChangeLog
        svn --non-interactive log >> DetailedChangeLog
    elif [ "$SCM" == "git" ]; then
        echo "Created ChangeLog from GIT" > DetailedChangeLog
        git-log --pretty="format:------------------------------------------------------------------------%n%h | %an | %ad | ...%n%n%s%n%n" --all >> DetailedChangeLog
        echo "------------------------------------------------------------------------" >> DetailedChangeLog
    else
        echo "\tWarning: No Detailed ChangeLog created!"
        rm -f DetailedChangeLog
    fi
}

abstract_change_log () {
    echo "Creating Abstract ChangeLog"
    TMPCL="${TMPLDIR}/tmp-changelog"
    echo -e "\n\nCL: Insert abstract ChangeLog here\nCL: Lines beginning with CL: are removed automaticly" > "$TMPCL"
    sed -E -e 's/(.*)/CL: \1/g' DetailedChangeLog >> "$TMPCL"
    ${VISUAL} "$TMPCL"
    DATE=`date`
    echo -e "===================\n$VERSION released on $DATE\n\n" >> ChangeLog
    sed -E -e '/^CL:.*/d' "$TMPCL" >> ChangeLog
    rm -f "$TMPCL"
}

change_log () {
    echo "Creating ChangeLog"
    scm_change_log
    abstract_change_log
}

echo "Make Dist..."

DOCDIR=doc
ORIGPATH=`pwd`
PKGDIR="${PACKAGE}-${VERSION}"
PKGFILE="${ORIGPATH}/${PKGDIR}.tar.bz2"
TMPPKGDIR="${TMPDIR}/${PKGDIR}"
if [ -z "$SCM" -a "$SCM" != "off" ]; then
    autodetect_scm
fi

info
if [ "$ONLYINFO" == "true" ]; then
    exit 0
fi

#create new directory for package work
if [ "$NO_CHANGELOG" != "0" ]; then
    change_log
fi
create_package_dir
if [ "$NO_TESTS" != "0" ]; then
    run_tests
fi
clean
set_version_file
if [ "$NO_DOC" != "0" ]; then
    download_doc
fi
create_package
remove_package_dir

echo "Done!! [See $PKGFILE]"
if [ "$NO_CHANGELOG" != "0" ]; then
    echo -e "\tYou should commit the updated Change Log!"
fi
