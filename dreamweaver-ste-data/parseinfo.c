#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>

void decryptPassword(char *, char *);

int main(int argc, char **argv) {

    xmlDoc *document;
    xmlNode *root, *first_child, *node;
    char *filename;
    char plainPass[15] = {'\0'};

    if (argc < 2) {
        fprintf(stderr, "Usage: %s filename.ste\n", argv[0]);
        return 1;
    }

    filename = argv[1];

    document = xmlReadFile(filename, NULL, XML_PARSE_NOBLANKS);
    root = xmlDocGetRootElement(document);

    first_child = root->children;
    for (node = first_child; node; node = node->next) {
        // Tries to find <serverlist>.
        if ((!xmlStrcmp(node->name, (const xmlChar *) "serverlist"))) {
            node = node->children;
            break;
        }
    }

    xmlChar *host, *user, *pw, *remoteroot;

    host = xmlGetProp(node, (const xmlChar *) "host");
    user = xmlGetProp(node, (const xmlChar *) "user");
    pw = xmlGetProp(node, (const xmlChar *) "pw");
    remoteroot = xmlGetProp(node, (const xmlChar *) "remoteroot");

    decryptPassword(plainPass, (char *) pw);

    fprintf(stdout, "host ---------: %s\n", host);
    fprintf(stdout, "user ---------: %s\n", user);
    fprintf(stdout, "plainPass ----: %s\n", plainPass);
    fprintf(stdout, "remoteroot ---: %s\n", remoteroot);

    xmlFreeDoc(document);

    return 0;
}


void decryptPassword(char * pass, char * hash) {

    char tmp[2] = {'\0'};
    unsigned int i;
    int num;


    for (i = 0; i < strlen(hash); i += 2) {
        // Get pairs of chars and parse both to base 16.
        tmp[0] = hash[i];
        tmp[1] = hash[i + 1];

        // The pair is an hex num. Let's make an int out of it.
        num = ((int) strtol(tmp, NULL, 16)) - (i / 2);

        // And then get the ascii char from the int value.
        sprintf(tmp, "%c", num);

        // Add char in tmp to pass arr/str.
        strcat(pass, tmp);
    }
}

/*
http://www.xmlsoft.org/tutorial/ar01s04.html
*/
