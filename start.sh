if [ -e "which cmake" ]; then
      echo "start Webserv";
      mkdir -p Eval && cd Eval/ && cmake ../ > /dev/null && make > /dev/null && ./Webserv;
else
      if [[ $OS == "OSX" ]]; then
          echo "brew install";
          @brew install cmake;
      fi
      echo "start Webserv";
      mkdir -p Eval && cd Eval/ && cmake ../ > /dev/null && make > /dev/null && ./Webserv;
fi
