
#include <gnome-keyring.h>

const char* gnome_keyring_result_to_message(GnomeKeyringResult result)
{
	switch (result) {
	case GNOME_KEYRING_RESULT_OK:
		return "OK";
	case GNOME_KEYRING_RESULT_DENIED:
		return "Denied";
	case GNOME_KEYRING_RESULT_NO_KEYRING_DAEMON:
		return "No Keyring Daemon";
	case GNOME_KEYRING_RESULT_ALREADY_UNLOCKED:
		return "Already UnLocked";
	case GNOME_KEYRING_RESULT_NO_SUCH_KEYRING:
		return "No Such Keyring";
	case GNOME_KEYRING_RESULT_BAD_ARGUMENTS:
		return "Bad Arguments";
	case GNOME_KEYRING_RESULT_IO_ERROR:
		return "IO Error";
	case GNOME_KEYRING_RESULT_CANCELLED:
		return "Cancelled";
	case GNOME_KEYRING_RESULT_ALREADY_EXISTS:
		return "Already Exists";
	}

	return "Unknown Error";
}

 int unlock_keyring()
{
	GnomeKeyringResult result;
	result = gnome_keyring_unlock_sync(GNOME_KEYRING_DEFAULT, "111111");

	if (result != GNOME_KEYRING_RESULT_OK) {
		g_critical("failed unlocking keyring: %s",
			gnome_keyring_result_to_message(result));
		return 1;
	}

	return 0;
}


int main() {
	unlock_keyring();
	return 0;
}
