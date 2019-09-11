# Configuration files used on the Beagle Bone

- `~/.bash_aliases`:

Stores general bash shortcuts for quicker access/sourcing to various files, navigation shortcuts, script aliases.

NOTE: Need to append `. ~/.bash_aliases` to `~/.bashrc` in order for this to get sourced everytime a new terminal session is started.

- `~/.nanorc`:

User level configuration file for nano text editor.

- `init_cape.sh`:

Must be run before calling any SPI functions. It can be configured to run on startup via the instructions found [here](https://gist.github.com/pdp7/d4770a6ba17e666848796bf5cfd0caee)

- `startup.sh`:

This activates the virtual environment and starts the flask server. The method used was to call it on boot with crontab.
