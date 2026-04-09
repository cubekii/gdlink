#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "openurl/searchtab.h"
#include <url.h>

using namespace geode::prelude;

$on_mod(Loaded) {
	CustomUrl gdlink("gdlink");
}