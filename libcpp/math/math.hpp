#pragma once

#include <libcpp/math/abs.hpp>
#include <libcpp/math/ceil.hpp>
#include <libcpp/math/floor.hpp>
#include <libcpp/math/metric.hpp>
#include <libcpp/math/sqrt.hpp>

#ifndef USE_OPENCV_MAT
#include <libcpp/math/matrix.hpp>
#endif

#ifdef USE_OPENCV_MAT
#include <libcpp/math/opencv/mat.hpp>
#endif