pkgname=uncso2
pkgver=2.1.1
pkgrel=1
pkgdesc='Extract Counter-Strike: Online 2 and Titanfall Online game files'
arch=(any)
url="https://github.com/harmonytf/UnCSO2"
license=(GPL-3.0-only)
depends=(libuncso2)
makedepends=(cmake)
source=()
_sourcedir="UnCSO2"

prepare() {
	ln -snf "$startdir" "$srcdir"
}

build() {
	cmake -B build -S "$_sourcedir" \
        -DCMAKE_BUILD_TYPE=RelWithDebInfo \
        -DCMAKE_INSTALL_PREFIX=/usr \
        -Wno-dev
    cmake --build build
}

package() {
	DESTDIR="$pkgdir" cmake --install build
	install -Dm644 "$_sourcedir/COPYING" -t "$pkgdir/usr/share/licenses/$pkgname"

	rm -rf "$pkgdir/usr/bin/cryptest"
	rm -rf "$pkgdir/usr/include"
	rm -rf "$pkgdir/usr/lib/libcryptopp.a"
	rm -rf "$pkgdir/usr/share/cmake"
	rm -rf "$pkgdir/usr/share/cryptopp"
	rm -rf "$pkgdir/usr/share/pkgconfig/cryptopp.pc"

	# those belong to libuncso2 package
	rm -rf "$pkgdir/usr/lib"
}
