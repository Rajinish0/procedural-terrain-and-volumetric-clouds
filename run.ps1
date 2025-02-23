mingw32-make
if ($?) {
	"build complete, running.."
	./main
} else {
	echo "something went south. Good luck debuging man"
}
