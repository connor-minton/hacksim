# Run this script in the root of the repository to create a .zip file for
# distributing the binaries and selected sample programs.
# Modify the VERSION variable as necessary.

set -e

VERSION=1.0.0

OUT=hacksim-win32-$VERSION

if [[ ! -f create-release.sh ]]; then
  echo error: run this script in the root of the repository 1>&2
  exit 1
fi

rm -rf $OUT

mkdir $OUT

cp src/hacksim.conf $OUT
cp Release/hacksim-win32.exe $OUT

mkdir -p $OUT/test_programs/asm
mkdir -p $OUT/test_programs/jack
cp test_programs/asm/Animation.hack $OUT/test_programs/asm
cp test_programs/asm/AnimationWait.hack $OUT/test_programs/asm
cp test_programs/asm/Fill.hack $OUT/test_programs/asm
cp test_programs/asm/Rect100.hack $OUT/test_programs/asm

cp test_programs/jack/HelloWorld/HelloWorld.hack $OUT/test_programs/jack
cp test_programs/jack/KeyboardTest/KeyboardTest.hack $OUT/test_programs/jack
cp test_programs/jack/Pong/Pong.hack $OUT/test_programs/jack
cp test_programs/jack/ScreenTest/ScreenTest.hack $OUT/test_programs/jack
cp test_programs/jack/WaitTest/WaitTest.hack $OUT/test_programs/jack

zip -r $OUT.zip $OUT

rm -rf $OUT