#include <stdbool.h>
#include <stdio.h>

#include  <gio/gio.h>
#include <libsecret/secret.h>

#define KEYRING_LOGIN "login"
#define PASSWORD_SECRET_VALUE_CONTENT_TYPE "text/plain"
#define COLLECTION_INTERFACE "org.freedesktop.Secret.Collection"

static bool is_default_keyring_exists(SecretService *service);

static void unlock_keyring(SecretService *service)
{
	SecretCollection *collection;
	GError *error = NULL;
	GList *unlocked;
	GList *objects;
	gboolean ret;

	collection = secret_collection_for_alias_sync(service,
                                                                    SECRET_COLLECTION_DEFAULT,
                                                                    SECRET_COLLECTION_NONE,
                                                                    NULL,
                                                                    &error);
	g_assert_no_error (error);
	printf("--get collection\n");

	objects = g_list_append (NULL, collection);
	ret = secret_service_unlock_sync(service, objects, NULL, &unlocked, &error);
	printf("--unlock ret:%d \n", ret);
	g_assert_no_error (error);
	g_assert_true (ret);

	g_assert_nonnull (unlocked);
	g_assert_true (unlocked->data == collection);
	g_assert_null (unlocked->next);
	g_list_free_full (unlocked, g_object_unref);

	g_list_free (objects);
	g_object_unref (collection);
}

static void lock_keyring(SecretService *service)
{
	SecretCollection *collection;
	GError *error = NULL;
	GList *locked;
	GList *objects;
	gboolean ret;

	collection = secret_collection_for_alias_sync(service,
                                                                    SECRET_COLLECTION_DEFAULT,
                                                                    SECRET_COLLECTION_NONE,
                                                                    NULL,
                                                                    &error);
	
	g_assert_no_error (error);

	objects = g_list_append (NULL, collection);

	ret = secret_service_lock_sync(service, objects, NULL, &locked, &error);
	g_assert_no_error (error);
	g_assert_true (ret);

	g_assert_nonnull (locked);
	g_assert_true (locked->data == collection);
	g_assert_null (locked->next);
	g_list_free_full (locked, g_object_unref);

	g_list_free (objects);
	g_object_unref (collection);
}


int main() {
    int res = 0;

    GError *err = NULL;
    SecretService *service = NULL;
    SecretCollection *collection = NULL;
    SecretValue *password = NULL;
    GDBusConnection *bus = NULL;
    GVariant *ret = NULL;

    do {
        service = secret_service_get_sync(SECRET_SERVICE_OPEN_SESSION, NULL, &err);
        if (service == NULL) {
            printf("failed to get secret service: %s\n", err->message);
            res = 1;
            break;
        }
/*
        password = secret_value_new("111111", 0, PASSWORD_SECRET_VALUE_CONTENT_TYPE);

        bus = g_bus_get_sync(G_BUS_TYPE_SESSION, NULL, &err);
        if (bus == NULL) {
            printf("failed to get session bus: %s\n", err->message);
            res = 1;
            break;
        }

        GVariant *label = g_variant_new_dict_entry(
                g_variant_new_string(COLLECTION_INTERFACE ".Label"),
                g_variant_new_variant(g_variant_new_string(KEYRING_LOGIN)));
        GVariant *attributes = g_variant_new_array(G_VARIANT_TYPE("{sv}"), &label, 1);
	*/

        if (is_default_keyring_exists(service)) {
		printf("----default keyring is exist\n");
		//unlock_keyring(service);
		lock_keyring(service);
       	}
    } while (false);

    return 0;
}

bool is_default_keyring_exists(SecretService *service) {
    GError *err = NULL;
    SecretCollection *collection = secret_collection_for_alias_sync(service,
                                                                    SECRET_COLLECTION_DEFAULT,
                                                                    SECRET_COLLECTION_NONE,
                                                                    NULL,
                                                                    &err);
    if (err != NULL) {
        printf("failed to get default secret collection: %s\n", err->message);
        g_error_free(err);
        return false;
    }
    if (collection == NULL) {
        printf("default secret collection not exists\n");
        return false;
    }

    g_object_unref(collection);
    return true;
}

