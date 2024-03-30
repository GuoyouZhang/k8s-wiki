# k8s-wiki
Record hands-on experience on k8s

how to pack java jar into window exe on arm linux with launch4j
  sudo apt-get install mingw-w64
  cd /home/ubuntu/launch4j/bin
  rm -rf ld windres
  ln -s /usr/bin/i686-w64-mingw32-windres windres
  ln -s /usr/bin/i686-w64-mingw32-ld ld
  cd ..
  ~/jdk1.8.0_391/bin/java -jar launch4j.jar demo/SimpleApp/l4j/SimpleApp.xml
  
  launch4j: Compiling resources
  launch4j: Linking
  launch4j: Successfully created /home/ubuntu/launch4j/demo/SimpleApp/l4j/../SimpleApp.exe
