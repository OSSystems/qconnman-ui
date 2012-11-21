# qconnman-ui - ConnMan GUI

![Screenshot](https://github.com/OSSystems/qconnman-ui/raw/gh-pages/images/screenshot.png "Screenshot")

## Compiling

Dependencies: Qt4, qconnman library (https://bitbucket.org/ossystems/qconnman)

Install qconnman library:

    git clone https://bitbucket.org/ossystems/qconnman.git
    cd qconnman
    qmake PREFIX=/usr
    make
    make install

Then compile qconnman-ui:

    cd qconnman-ui
    qmake && make
    ./manager/qconnman-ui

## Authors

* Luis Gustavo S. Barreto (gustavo@ossystems.com.br/gustavosbarreto@gmail.com)

## License

Licensed under GNU Lesser General Public License 2.1
