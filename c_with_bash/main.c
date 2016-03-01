#include <stdio.h>

#ifdef NEVER
  case $0 in
  *.c)  ;;
  bash) echo 'Use "bash *.c" and not "bash < *.c"' >&2; exit 1;;
  sh)   echo 'Use "sh *.c" and not "sh < *.c"' >&2; exit 1;;
  *)    echo 'Filename must end in ".c"' >&2; exit 1;;
  esac
  
  build=0; run=0; clean=0;
  case $1 in
  "")     build=1 ; run=1;;
  build)  build=1;;
  run)    run=1;;
  clean)  clean=1;;
  *)      echo 'Unrecognized command' >&2; exit 1;;
  esac
  
  if [ "$build" -ne "0" ] ; then
    echo Compiling $0:
    echo ${CC=cc} ${0} -o ${0%.*}
    ${CC=cc} ${0} -o ${0%.*} 
  fi
  
  if [ "$run" -ne "0" ]; then
    echo 
    echo output:
    ${0%.*}
  fi
  
  if [ "$clean" -ne "0" ]; then
    rm -f ${0%.*}
  fi
  
  exit 0
#endif


int main(void) {
  printf("hello world\n");
  return 0;
}
