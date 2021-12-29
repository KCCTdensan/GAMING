import std;
import vibe.vibe;
import dlog;
import gaming.hidapi;

enum DefaultPort = 8080;

void main() {
  auto logger = new DefaultLogger();
  logger.log("booting up...");
  scope(exit) logger.log("exit...");

  auto router = new URLRouter;
  router.get("/", &handler);

  auto settings = new HTTPServerSettings;
  settings.port = DefaultPort;

  listenHTTP(settings, &handler);

  runApplication();
}

void handler(HTTPServerRequest req, HTTPServerResponse res) {
}
