#pragma once

#include "string.hpp"
#include "point.hpp"
#include "point3d.hpp"
#include "variant.hpp"
#include "pair.hpp"
#include "vector.hpp"
#include "vector2d.hpp"
#include "vector3d.hpp"
#include "list.hpp"
#include "queue.hpp"
#include "stack.hpp"
#include "b_tree.hpp"
#include "map.hpp"

namespace azh::sdk::type
{
    using stringlist = list<std::string>;
    using variantlist = list<variant>;
}