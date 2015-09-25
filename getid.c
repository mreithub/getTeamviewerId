#include <windows.h>
#include <stdio.h>

BOOL CALLBACK findWindowCallback(HWND hwnd, LPARAM lParam);
BOOL CALLBACK findTextboxCallback(HWND hwnd, LPARAM lParam);

BOOL isTeamviewerId(char *buff);
BOOL isTeamviewerPassword(char *buff);

const char *TV_WINDOW_TITLES[] = {
	"TeamViewer QuickSupport",
	"TeamViewer",
	NULL
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int iCmdShow) {
	char id[30] = {0};
	int rc;

	EnumWindows(findWindowCallback, (LPARAM) id);

	if (strlen(id) > 0) {
		printf("%s\n", id);
		rc = 0;
	}
	else rc = 1;

	return rc;
}

BOOL CALLBACK findWindowCallback(HWND hwnd, LPARAM lParam) {
	char className[80];
	char title[80];
	int i;

	GetClassName(hwnd, className, sizeof(className));
	GetWindowText(hwnd, title, sizeof(title));


	for (i=0;TV_WINDOW_TITLES[i] != NULL;i++) {
		if (!strncmp(title, TV_WINDOW_TITLES[i], strlen(TV_WINDOW_TITLES[i])+1)) {
			// found TeamViewer
			//fprintf(stderr, "Found %s window: hWnd=%d (class=%s)\n", title, hwnd, className);
			EnumChildWindows(hwnd, findTextboxCallback, lParam);
		}
	}

	return TRUE;
}

BOOL CALLBACK findTextboxCallback(HWND hwnd, LPARAM lParam) {
	const char *cmd = getenv("CMD");
	char text[80];
	char className[80];
	BOOL rc = TRUE;

	//GetWindowText(hwnd, text, sizeof(text));
	GetClassName(hwnd, className, sizeof(className));
	SendMessage(hwnd, WM_GETTEXT, (WPARAM) sizeof(text), (LPARAM) text);

	if (!strcmp(cmd, "pwd")) {
		if (isTeamviewerPassword(text)) {
			//fprintf(stderr, "Found password: %s (hWnd=%d, class=%s)\n", text, hwnd, className);
			if (lParam != 0) {
				strncpy((char*) lParam, text, 20);
			}
			rc = FALSE;
		}
	}
	else if (!strcmp(cmd, "texts")) {
		printf("%s\n", text);
	}
	else {
		if (isTeamviewerId(text)) {
			//fprintf(stderr, "Found ID: %s (hWnd=%d, class=%s)\n", text, hwnd, className);
			if (lParam != 0) {
				strncpy((char*) lParam, text, 20);
			}
			rc = FALSE;
		}
	}

	return rc;
}

BOOL isTeamviewerId(char *buff) {
	int idParts[3], i;
	BOOL rc = FALSE;

//	printf("isId: %s\n", buff);
	if (3 == sscanf(buff, "%d %d %d", &idParts[0], &idParts[1], &idParts[2])) {
		rc = TRUE;
		for (i = 0; i < sizeof(idParts)/sizeof(idParts[0]); i++) {
			if (idParts[i] < 0 || idParts[i] > 999) rc = FALSE;
		}
	}

	return rc;
}

BOOL isTeamviewerPassword(char *buff) {
	char *endPtr = NULL;
	BOOL rc = FALSE;

	int pw = strtol(buff, &endPtr, 10);

	if (*buff != '\0' && *endPtr == '\0') {
		if (pw > 0 && pw < 1000000) { // allow numeric passwords of up to 6 digits
			rc = TRUE;
		}
	}

	return rc;
}
