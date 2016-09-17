# inquisition_networking - Netquisition
I'm sure you didn't expect the Spanish Inquisition.  
## WTF is that?
This is simple client-server C++ library that uses Merkle's Puzzles to authenticate
and inquisition_cipher to encrypt data.
## Why to use it?
Idk. If implementing SSL is too hard for you, or you just don't wanna waste processor
time on calculating huge numbers, Netquisition is right for you. It performs a lot more
operations on memory than on integers. Well, it's still WIP, so it may not work properly
(right now it just doesn't work. how sad is that.).
## Depencies
Clone, compile and install inquisition_cipher from
```
https://github.com/majudev/inquisition_cipher.git
```
## Compilation
Just run
```
./make.sh
```
to build and
```
./gen.sh
```
to generate Debian packages.
## Linking
Just add `-lnetquisition -linquisition` to your linker options.
## Samples
Samples are `src/test*` files. They are built by `./make.sh` and stored in `bin` catalog.
## License
License can be found in LICENSE file.
## Links
* [Merkle's Puzzles](https://en.wikipedia.org/wiki/Merkle's_Puzzles)
* [inquisition_cipher](https://github.com/majudev/inquisition_cipher)
