#!/bin/bash -e
#
# Helper script to run teamviewer-getid.exe on linux
#
# exit codes:
#  0: success
#  1: failure in .exe
#  2: couldn't get WINEPREFIX (maybe teamviewer's not running right now)
#  3: couldn't find wine binary in the teamviewer directory

usage() {
	cat >&2 <<EOF
USAGE: $0 [--id|--pwd|--texts]" >&1

	--id (default): Print the ID
	--pwd: Print the numeric session password
	--texts: print all text fields' contents, line by line
EOF
}

# TODO do proper argument handling
if [ -z "$1" -o '--id' == "$1" ]; then
	true #CMD={null}
elif [ '--texts' == "$1" ]; then
	CMD='texts'
elif [ '--pwd' == "$1" ]; then
	CMD='pwd'
else
	usage
	exit 1
fi

# get $WINEPREFIX from the currently running teamviewer instance
tvPid="$(pgrep TeamViewer.exe||true)"

if [ -z "$tvPid" ]; then
	echo "Error: TeamViewer not running!" >&2
	exit 2
fi

export "WINEPREFIX=$(cat "/proc/$tvPid/environ" | tr '\0' '\n' | grep '^WINEPREFIX=' | sed 's/^[^=]*=//')"

# read the /usr/bin/teamviewer link
tvDir="$(readlink /usr/bin/teamviewer|sed 's|\(^/[^/]*/[^/]*/\).*$|\1|')"
winePath="$(find "$tvDir" -type f -name wine)"

if [ -z "$winePath" ]; then
	echo "Error: Couldn't find wine binary (tvDir='$tvDir')"
	exit 3
fi

# get the path of the getId.exe
getIdExe="$(dirname "$0")/teamviewer-getid.exe"

#echo "wine: $WINEPREFIX"
#echo "tvDir: $tvDir"
#echo "wine: $winePath"
#echo "exe: $getIdExe"

# run the .exe in the right wineprefix (which we already exported above)

"$winePath" "$getIdExe"
exit "$?"
