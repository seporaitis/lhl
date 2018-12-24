# Linux Ham Log

An attempt at writing a modern terminal ham radio logging software for Linux from scratch.

# Requirements

- `ncurses`
- `autoconf`
- `automake`

# Compile & Run

```
autoreconf -i
./configure
make
src/lhl
```

# Ethos

Start simple and gradually add additional features if there is an interest.

# Ideas

List of minimum requirements:

- come up with a normal name for this project
- allow basic input about a qso
  - timestamp (date + time)
  - mode
  - band
  - callsign
  - rst sent
  - rst rcvd
- save and load log files
- show these inputs in a list on screen
- support log files

Anything else can go afterwards:

- additional inputs
  - frequency
  - power
  - qth
  - name
  - comment
- additional information about a qso
  - country
- search
- read data from rig (power, frequency)
- contest mode
- control rig
