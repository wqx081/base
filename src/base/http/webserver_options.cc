#include "base/http/webserver_options.h"

#include <string>
#include <gflags/gflags.h>
#include <string.h>
#include <stdlib.h>

#include "base/core/strings/substitute.h"

using std::string;

namespace base {

static std::string GetDefaultDocumentRoot();

} // namespace base

// Flags defining web server behavior. The class implementation should
// not use these directly, but rather access them via WebserverOptions.
// This makes it easier to instantiate web servers with different options
// within a single unit test.
DEFINE_string(webserver_interface, "",
 "Interface to start debug webserver on. "
 "If blank, webserver binds to 0.0.0.0");

DEFINE_string(webserver_doc_root, base::GetDefaultDocumentRoot(),
"Files under <webserver_doc_root> are accessible via the debug webserver. "
"Defaults to $MPR_HOME/www, or if $MPR_HOME is not set, disables the document "
"root");

DEFINE_bool(webserver_enable_doc_root, true,
"If true, webserver may serve static files from the webserver_doc_root");

DEFINE_string(webserver_certificate_file, "",
"The location of the debug webserver's SSL certificate file, in .pem format. If "
"empty, webserver SSL support is not enabled");
DEFINE_string(webserver_authentication_domain, "",
"Domain used for debug webserver authentication");

DEFINE_string(webserver_password_file, "",
"(Optional) Location of .htpasswd file containing user names and hashed passwords for"
" debug webserver authentication");


DEFINE_int32(webserver_num_worker_threads, 50,
"Maximum number of threads to start for handling web server requests");

DEFINE_int32(webserver_port, 0,
             "Port to bind to for the web server");

namespace base {

// Returns MPR_HOME if set, otherwise we won't serve any static files.
static string GetDefaultDocumentRoot() {
  char* mpr_home = getenv("MPR_HOME");
  // Empty document root means don't serve static files
  return mpr_home ? strings::Substitute("$0/www", mpr_home) : "";
}

WebserverOptions::WebserverOptions()
  : bind_interface(FLAGS_webserver_interface),
    port(FLAGS_webserver_port),
    doc_root(FLAGS_webserver_doc_root),
    enable_doc_root(FLAGS_webserver_enable_doc_root),
    certificate_file(FLAGS_webserver_certificate_file),
    authentication_domain(FLAGS_webserver_authentication_domain),
    password_file(FLAGS_webserver_password_file),
    num_worker_threads(FLAGS_webserver_num_worker_threads) {
}

} // namespace base
