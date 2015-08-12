![Arbore](https://raw.githubusercontent.com/MichaelMure/Arbore-qt/master/resources/logo/arbore-prelogo.png)

Here shall appear a friend-to-friend filesharing app with the following caracteristics:
- targeted for private/low diffusion data between people that know each-others. Main use case: friends want to share between each-others their holidays pictures.
- low friction: it should be as easy as possible
- encryption should be used to maintain privacy and to control the diffusion of the data

This application will be build on top of [IPFS](http://ipfs.io/).

Here is what we envision currently ([link](https://raw.githubusercontent.com/MichaelMure/Arbore-qt/master/resources/mockups/arboreUImockups-june-w.png)):
![Mockups](https://raw.githubusercontent.com/MichaelMure/Arbore-qt/master/resources/mockups/arboreUImockups-june-w.png)

## How to build

### On Debian like

#### Dependencies

	# apt-get install libqt5declarative5 qml qml-module-qtquick-controls qt5-qmake qtdeclarative5-dev

#### Configure build and run

	$ cd src
	$ mkdir build
	$ cd build
	$ qmake ..
	$ make -j $(nproc)
	$ ./Arbore-qt

Or, simply using the provided Makefile:

	$ cd src
	$ make -j $(proc) run

In order to run in debug mode

	$ cd src/build
	$ make debug

## How to help

Every little help. Please contact us if you want to build something awesome, especially if you know UX or Qt/QML.

## Want to talk ?

You can drop by our IRC channel, #arbore on freenode.
