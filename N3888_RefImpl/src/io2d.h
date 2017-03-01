#pragma once

#ifndef _IO2D_
#define _IO2D_

#define __cpp_lib_experimental_io2d 201602

#include "xio2d.h"

#include <memory>
#include <functional>
#include <exception>
#include <vector>
#include <string>
#include <algorithm>
#include <system_error>
#include <cstdint>
#include <atomic>
#include <variant>
#include <optional>
#include <cmath>
#include <type_traits>
#include <initializer_list>

#ifdef _WIN32_WINNT
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#elif defined(USE_XCB)
#include <xcb/xcb.h>
#elif defined(USE_XLIB)
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#endif

#if !_Noexcept_conditional_support_test
#define noexcept
#endif

namespace std {
	namespace experimental {
		namespace io2d {
#if _Inline_namespace_conditional_support_test
			inline namespace v1 {
#endif
				enum class io2d_error {
					success,
					invalid_restore,
					invalid_matrix,
					invalid_status,
					null_pointer,
					invalid_string,
					invalid_path_data,
					read_error,
					write_error,
					surface_finished,
					invalid_dash,
					invalid_index,
					clip_not_representable,
					invalid_stride,
					user_font_immutable,
					user_font_error,
					invalid_clusters,
					device_error,
					invalid_mesh_construction
				};
#if _Inline_namespace_conditional_support_test
			}
#endif
		}
	}

	template<>
	struct is_error_condition_enum<::std::experimental::io2d::io2d_error>
		: public ::std::true_type { };

	//template<>
	//struct is_error_code_enum<::cairo_status_t>
	//	: public ::std::true_type{ };

	::std::error_condition make_error_condition(experimental::io2d::io2d_error e) noexcept;

	//	::std::error_code make_error_code(cairo_status_t e) noexcept;

	::std::error_code make_error_code(experimental::io2d::io2d_error e) noexcept;

	namespace experimental {
		namespace io2d {
#if _Inline_namespace_conditional_support_test
			inline namespace v1 {
#endif
				enum class antialias {
					default_antialias,
					none,
					gray,
					subpixel,
					fast,
					good,
					best
				};

				enum class content {
					color,
					alpha,
					color_alpha
				};

				enum class fill_rule {
					winding,
					even_odd
				};

				enum class line_cap {
					butt,
					round,
					square
				};

				enum class line_join {
					miter,
					round,
					bevel,
					miter_or_bevel
				};

				enum class compositing_operator {
					over,
					clear,
					source,
					in,
					out,
					atop,
					dest,
					dest_over,
					dest_in,
					dest_out,
					dest_atop,
					xor_op,
					add,
					saturate,
					multiply,
					screen,
					overlay,
					darken,
					lighten,
					color_dodge,
					color_burn,
					hard_light,
					soft_light,
					difference,
					exclusion,
					hsl_hue,
					hsl_saturation,
					hsl_color,
					hsl_luminosity
				};

				enum class format {
					invalid,
					argb32,
					xrgb32,
					a8,
					a1,
					rgb16_565,
					rgb30
				};

				enum class tiling {
					none,
					repeat,
					reflect,
					pad
				};

				enum class filter {
					fast,
					good,
					best,
					nearest,
					bilinear
				};

				enum class brush_type {
					solid_color,
					surface,
					linear,
					radial
				};

				enum class font_slant {
					normal,
					italic,
					oblique
				};

				enum class font_weight {
					normal,
					bold
				};

				enum class subpixel_order {
					default_subpixel_order,
					horizontal_rgb,
					horizontal_bgr,
					vertical_rgb,
					vertical_bgr
				};

				enum class scaling {
					letterbox, // Same as uniform except that the display_surface is cleared using the letterbox brush first
					uniform, // Maintain aspect ratio and center as needed, ignoring side areas that are not drawn to
					fill_uniform, // Maintain aspect ratio but fill entire display (some content may not be shown)
					fill_exact, // Ignore aspect ratio and use (possibly non-uniform) scale to fill exactly
					none // Do not scale.
				};

				enum class refresh_rate {
					as_needed,
					as_fast_as_possible,
					fixed
				};

				struct nullvalue_t {
					constexpr explicit nullvalue_t(int) noexcept {
					}
				};

				constexpr nullvalue_t nullvalue{ 0 };

#if _Variable_templates_conditional_support_test
				template <class T>
				constexpr T pi = T(3.14159265358979323846264338327950288L);

				template <class T>
				constexpr T two_pi = T(6.28318530717958647692528676655900576L);

				template <class T>
				constexpr T half_pi = T(1.57079632679489661923132169163975144L);

				template <class T>
				constexpr T three_pi_over_two = T(4.71238898038468985769396507491925432L);
#else
				template <class T>
#if _Constexpr_conditional_support_test
				constexpr
#endif
					T pi() noexcept {
					return static_cast<T>(3.14159265358979323846264338327950288L);
				}

				template <class T>
#if _Constexpr_conditional_support_test
				constexpr
#endif
					T two_pi() noexcept {
					return static_cast<T>(6.28318530717958647692528676655900576L);
				}

				template <class T>
#if _Constexpr_conditional_support_test
				constexpr
#endif
					T half_pi() noexcept {
					return static_cast<T>(1.57079632679489661923132169163975144L);
				}

				template <class T>
#if _Constexpr_conditional_support_test
				constexpr
#endif
					T three_pi_over_two() noexcept {
					return static_cast<T>(4.71238898038468985769396507491925432L);
				}
#endif

				// I don't know why Clang/C2 is complaining about weak vtables here since the at least one virtual function is always anchored but for now silence the warnings. I've never seen this using Clang on OpenSUSE.
#ifdef _WIN32
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif
#endif

				class io2d_error_category : public ::std::error_category {
				public:
					// Observers
					virtual const char* name() const noexcept override;
					virtual ::std::string message(int errVal) const override;
					virtual bool equivalent(int code, const ::std::error_condition& condition) const noexcept override;
					virtual bool equivalent(const ::std::error_code& ec, int condition) const noexcept override;
				};

				// I don't know why Clang/C2 is complaining about weak vtables here since the at least one virtual function is always anchored but for now silence the warnings. I've never seen this using Clang on OpenSUSE.
#ifdef _WIN32
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#endif

				const ::std::error_category& io2d_category() noexcept;

				class vector_2d {
					double _X = 0.0;
					double _Y = 0.0;
				public:
					constexpr vector_2d() noexcept { }
					constexpr vector_2d(double x, double y) noexcept
						: _X(x)
						, _Y(y) {
					}
					constexpr vector_2d(const vector_2d&) noexcept = default;
					vector_2d(vector_2d&&) noexcept = default;
					constexpr vector_2d& operator=(const vector_2d&) noexcept = default;
					vector_2d& operator=(vector_2d&&) noexcept = default;

					void x(double value) noexcept;
					void y(double value) noexcept;
					void swap(vector_2d& x) noexcept;

					constexpr double x() const noexcept {
						return _X;
					}
					constexpr double y() const noexcept {
						return _Y;
					}

					double magnitude() const noexcept;
					constexpr double magnitude_squared() const noexcept {
						return _X * _X + _Y * _Y;
					}
					constexpr double dot(const vector_2d& other) const noexcept {
						return _X * other._X + _Y * other._Y;
					}
					double angular_direction() const noexcept;
					vector_2d to_unit() const noexcept;

					vector_2d& operator+=(const vector_2d& rhs) noexcept;
					vector_2d& operator-=(const vector_2d& rhs) noexcept;
					vector_2d& operator*=(double rhs) noexcept;
				};

				inline constexpr bool operator==(const vector_2d& lhs, const vector_2d& rhs) noexcept {
					return lhs.x() == rhs.x() && lhs.y() == rhs.y();
				}

				inline constexpr bool operator!=(const vector_2d& lhs, const vector_2d& rhs) noexcept {
					return !(lhs == rhs);
				}

				inline constexpr vector_2d operator+(const vector_2d& lhs) noexcept {
					return lhs;
				}

				inline constexpr vector_2d operator+(const vector_2d& lhs, const vector_2d& rhs) noexcept {
					return vector_2d{ lhs.x() + rhs.x(), lhs.y() + rhs.y() };
				}

				inline vector_2d& vector_2d::operator+=(const vector_2d& rhs) noexcept {
					_X = _X + rhs.x();
					_Y = _Y + rhs.y();
					return *this;
				}

				inline constexpr vector_2d operator-(const vector_2d& lhs) noexcept {
					return vector_2d{ -lhs.x(), -lhs.y() };
				}

				inline constexpr vector_2d operator-(const vector_2d& lhs, const vector_2d& rhs) noexcept {
					return vector_2d{ lhs.x() - rhs.x(), lhs.y() - rhs.y() };
				}

				inline vector_2d& vector_2d::operator-=(const vector_2d& rhs) noexcept {
					_X = _X - rhs.x();
					_Y = _Y - rhs.y();
					return *this;
				}

				inline vector_2d& vector_2d::operator*=(double rhs) noexcept {
					_X *= rhs;
					_Y *= rhs;
					return *this;
				}

				inline constexpr vector_2d operator*(const vector_2d& lhs, double rhs) noexcept {
					return vector_2d{ lhs.x() * rhs, lhs.y() * rhs };
				}

				inline constexpr vector_2d operator*(double lhs, const vector_2d& rhs) noexcept {
					return vector_2d{ lhs * rhs.x(), lhs * rhs.y() };
				}

				//void swap(vector_2d& lhs, vector_2d& rhs);

				class rectangle {
					double _X = 0.0;
					double _Y = 0.0;
					double _Width = 0.0;
					double _Height = 0.0;
				public:
					constexpr rectangle() noexcept { }
					constexpr rectangle(double x, double y, double width, double height) noexcept
						: _X(x)
						, _Y(y)
						, _Width(width)
						, _Height(height) {
					}
					constexpr rectangle(const vector_2d& tl, const vector_2d& br) noexcept
						: _X(tl.x())
						, _Y(tl.y())
						, _Width(::std::max(0.0, br.x() - tl.x()))
						, _Height(::std::max(0.0, br.y() - tl.y())) {
					}
					constexpr rectangle(const rectangle&) noexcept = default;
					constexpr rectangle& operator=(const rectangle&) noexcept = default;
					rectangle(rectangle&&) noexcept = default;
					rectangle& operator=(rectangle&&) noexcept = default;


					void x(double value) noexcept;
					void y(double value) noexcept;
					void width(double value) noexcept;
					void height(double value) noexcept;
					void top_left(const vector_2d& value) noexcept;
					void bottom_right(const vector_2d& value) noexcept;
					void top_left_bottom_right(const vector_2d& tl, const vector_2d& br) noexcept;

					constexpr double x() const noexcept;
					constexpr double y() const noexcept;
					constexpr double width() const noexcept;
					constexpr double height() const noexcept;
					constexpr double left() const noexcept;
					constexpr double right() const noexcept;
					constexpr double top() const noexcept;
					constexpr double bottom() const noexcept;
					constexpr vector_2d top_left() const noexcept;
					constexpr vector_2d bottom_right() const noexcept;
				};

				inline constexpr double rectangle::x() const noexcept {
					return _X;
				}

				inline constexpr double rectangle::y() const noexcept {
					return _Y;
				}

				inline constexpr double rectangle::width() const noexcept {
					return _Width;
				}

				inline constexpr double rectangle::height() const noexcept {
					return _Height;
				}

				inline constexpr double rectangle::left() const noexcept {
					return _X;
				}

				inline constexpr double rectangle::right() const noexcept {
					return _X + _Width;
				}

				inline constexpr double rectangle::top() const noexcept {
					return _Y;
				}

				inline constexpr double rectangle::bottom() const noexcept {
					return _Y + _Height;
				}

				inline constexpr vector_2d rectangle::top_left() const noexcept {
					return{ _X, _Y };
				}

				inline constexpr vector_2d rectangle::bottom_right() const noexcept {
					return{ _X + _Width, _Y + _Height };
				}

				class circle {
					vector_2d _Center;
					double _Radius = 0.0;
				public:
					constexpr circle() noexcept { }
					constexpr circle(const vector_2d& ctr, double rad) noexcept
						: _Center(ctr)
						, _Radius(rad) {
					}
					constexpr circle(const circle&) noexcept = default;
					constexpr circle& operator=(const circle&) noexcept = default;
					circle(circle&&) noexcept = default;
					circle& operator=(circle&&) noexcept = default;

					void center(const vector_2d& ctr) noexcept;
					void radius(double rad) noexcept;

					constexpr vector_2d center() const noexcept {
						return _Center;
					}

					constexpr double radius() const noexcept {
						return _Radius;
					}
				};

				class ellipse {
					vector_2d _Center;
					double _X_axis = 0.0;
					double _Y_axis = 0.0;
				public:
					constexpr ellipse() noexcept { }
					constexpr ellipse(const vector_2d& ctr, double xaxis, double yaxis) noexcept
						: _Center(ctr)
						, _X_axis(xaxis)
						, _Y_axis(yaxis) {
					}
					constexpr explicit ellipse(const circle& c) noexcept
						: _Center(c.center())
						, _X_axis(c.radius())
						, _Y_axis(c.radius()) { }
					constexpr ellipse(const ellipse&) noexcept = default;
					constexpr ellipse& operator=(const ellipse&) noexcept = default;
					ellipse(ellipse&&) noexcept = default;
					ellipse& operator=(ellipse&&) noexcept = default;

					void center(const vector_2d& ctr) noexcept {
						_Center = ctr;
					}
					void x_axis(double val) noexcept {
						_X_axis = val;
					}
					void y_axis(double val) noexcept {
						_Y_axis = val;
					}

					constexpr vector_2d center() const noexcept {
						return _Center;
					}

					constexpr double x_axis() const noexcept {
						return _X_axis;
					}

					constexpr double y_axis() const noexcept {
						return _Y_axis;
					}
				};

				class rgba_color {
					double _R = 0.0;
					double _G = 0.0;
					double _B = 0.0;
					double _A = 1.0;
				public:
					constexpr rgba_color() noexcept { }
					constexpr rgba_color(const rgba_color& other) noexcept = default;
					constexpr rgba_color& operator=(const rgba_color& other) noexcept = default;
					rgba_color(rgba_color&& other) noexcept = default;
					rgba_color& operator=(rgba_color&& other) noexcept = default;
					template <class T, ::std::enable_if_t<::std::is_same_v<double, ::std::remove_cv_t<::std::remove_reference_t<T>>> || ::std::is_same_v<float, ::std::remove_cv_t<::std::remove_reference_t<T>>>, int> = 0>
					constexpr rgba_color(T red, T green, T blue, T alpha = 1.0)
						: _R(static_cast<double>(::std::min<T>(::std::max<T>(red, 0.0), 1.0)))
						, _G(static_cast<double>(::std::min<T>(::std::max<T>(green, 0.0), 1.0)))
						, _B(static_cast<double>(::std::min<T>(::std::max<T>(blue, 0.0), 1.0)))
						, _A(static_cast<double>(::std::min<T>(::std::max<T>(alpha, 0.0), 1.0))) {
					}

					void r(double val);
					void r(double val, ::std::error_code& ec) noexcept;

					void g(double val);
					void g(double val, ::std::error_code& ec) noexcept;

					void b(double val);
					void b(double val, ::std::error_code& ec) noexcept;

					void a(double val);
					void a(double val, ::std::error_code& ec) noexcept;

					constexpr double r() const noexcept {
						return _R;
					}
					constexpr double g() const noexcept {
						return _G;
					}
					constexpr double b() const noexcept {
						return _B;
					}
					constexpr double a() const noexcept {
						return _A;
					}

					static const rgba_color& alice_blue() noexcept;
					static const rgba_color& antique_white() noexcept;
					static const rgba_color& aqua() noexcept;
					static const rgba_color& aquamarine() noexcept;
					static const rgba_color& azure() noexcept;
					static const rgba_color& beige() noexcept;
					static const rgba_color& bisque() noexcept;
					static const rgba_color& black() noexcept;
					static const rgba_color& blanched_almond() noexcept;
					static const rgba_color& blue() noexcept;
					static const rgba_color& blue_violet() noexcept;
					static const rgba_color& brown() noexcept;
					static const rgba_color& burly_wood() noexcept;
					static const rgba_color& cadet_blue() noexcept;
					static const rgba_color& chartreuse() noexcept;
					static const rgba_color& chocolate() noexcept;
					static const rgba_color& coral() noexcept;
					static const rgba_color& cornflower_blue() noexcept;
					static const rgba_color& cornsilk() noexcept;
					static const rgba_color& crimson() noexcept;
					static const rgba_color& cyan() noexcept;
					static const rgba_color& dark_blue() noexcept;
					static const rgba_color& dark_cyan() noexcept;
					static const rgba_color& dark_goldenrod() noexcept;
					static const rgba_color& dark_gray() noexcept;
					static const rgba_color& dark_green() noexcept;
					static const rgba_color& dark_grey() noexcept;
					static const rgba_color& dark_khaki() noexcept;
					static const rgba_color& dark_magenta() noexcept;
					static const rgba_color& dark_olive_green() noexcept;
					static const rgba_color& dark_orange() noexcept;
					static const rgba_color& dark_orchid() noexcept;
					static const rgba_color& dark_red() noexcept;
					static const rgba_color& dark_salmon() noexcept;
					static const rgba_color& dark_sea_green() noexcept;
					static const rgba_color& dark_slate_blue() noexcept;
					static const rgba_color& dark_slate_gray() noexcept;
					static const rgba_color& dark_slate_grey() noexcept;
					static const rgba_color& dark_turquoise() noexcept;
					static const rgba_color& dark_violet() noexcept;
					static const rgba_color& deep_pink() noexcept;
					static const rgba_color& deep_sky_blue() noexcept;
					static const rgba_color& dim_gray() noexcept;
					static const rgba_color& dim_grey() noexcept;
					static const rgba_color& dodger_blue() noexcept;
					static const rgba_color& firebrick() noexcept;
					static const rgba_color& floral_white() noexcept;
					static const rgba_color& forest_green() noexcept;
					static const rgba_color& fuchsia() noexcept;
					static const rgba_color& gainsboro() noexcept;
					static const rgba_color& ghost_white() noexcept;
					static const rgba_color& gold() noexcept;
					static const rgba_color& goldenrod() noexcept;
					static const rgba_color& gray() noexcept;
					static const rgba_color& green() noexcept;
					static const rgba_color& green_yellow() noexcept;
					static const rgba_color& grey() noexcept;
					static const rgba_color& honeydew() noexcept;
					static const rgba_color& hot_pink() noexcept;
					static const rgba_color& indian_red() noexcept;
					static const rgba_color& indigo() noexcept;
					static const rgba_color& ivory() noexcept;
					static const rgba_color& khaki() noexcept;
					static const rgba_color& lavender() noexcept;
					static const rgba_color& lavender_blush() noexcept;
					static const rgba_color& lawn_green() noexcept;
					static const rgba_color& lemon_chiffon() noexcept;
					static const rgba_color& light_blue() noexcept;
					static const rgba_color& light_coral() noexcept;
					static const rgba_color& light_cyan() noexcept;
					static const rgba_color& light_goldenrod_yellow() noexcept;
					static const rgba_color& light_gray() noexcept;
					static const rgba_color& light_green() noexcept;
					static const rgba_color& light_grey() noexcept;
					static const rgba_color& light_pink() noexcept;
					static const rgba_color& light_salmon() noexcept;
					static const rgba_color& light_sea_green() noexcept;
					static const rgba_color& light_sky_blue() noexcept;
					static const rgba_color& light_slate_gray() noexcept;
					static const rgba_color& light_slate_grey() noexcept;
					static const rgba_color& light_steel_blue() noexcept;
					static const rgba_color& light_yellow() noexcept;
					static const rgba_color& lime() noexcept;
					static const rgba_color& lime_green() noexcept;
					static const rgba_color& linen() noexcept;
					static const rgba_color& magenta() noexcept;
					static const rgba_color& maroon() noexcept;
					static const rgba_color& medium_aquamarine() noexcept;
					static const rgba_color& medium_blue() noexcept;
					static const rgba_color& medium_orchid() noexcept;
					static const rgba_color& medium_purple() noexcept;
					static const rgba_color& medium_sea_green() noexcept;
					static const rgba_color& medium_slate_blue() noexcept;
					static const rgba_color& medium_spring_green() noexcept;
					static const rgba_color& medium_turquoise() noexcept;
					static const rgba_color& medium_violet_red() noexcept;
					static const rgba_color& midnight_blue() noexcept;
					static const rgba_color& mint_cream() noexcept;
					static const rgba_color& misty_rose() noexcept;
					static const rgba_color& moccasin() noexcept;
					static const rgba_color& navajo_white() noexcept;
					static const rgba_color& navy() noexcept;
					static const rgba_color& old_lace() noexcept;
					static const rgba_color& olive() noexcept;
					static const rgba_color& olive_drab() noexcept;
					static const rgba_color& orange() noexcept;
					static const rgba_color& orange_red() noexcept;
					static const rgba_color& orchid() noexcept;
					static const rgba_color& pale_goldenrod() noexcept;
					static const rgba_color& pale_green() noexcept;
					static const rgba_color& pale_turquoise() noexcept;
					static const rgba_color& pale_violet_red() noexcept;
					static const rgba_color& papaya_whip() noexcept;
					static const rgba_color& peach_puff() noexcept;
					static const rgba_color& peru() noexcept;
					static const rgba_color& pink() noexcept;
					static const rgba_color& plum() noexcept;
					static const rgba_color& powder_blue() noexcept;
					static const rgba_color& purple() noexcept;
					static const rgba_color& red() noexcept;
					static const rgba_color& rosy_brown() noexcept;
					static const rgba_color& royal_blue() noexcept;
					static const rgba_color& saddle_brown() noexcept;
					static const rgba_color& salmon() noexcept;
					static const rgba_color& sandy_brown() noexcept;
					static const rgba_color& sea_green() noexcept;
					static const rgba_color& sea_shell() noexcept;
					static const rgba_color& sienna() noexcept;
					static const rgba_color& silver() noexcept;
					static const rgba_color& sky_blue() noexcept;
					static const rgba_color& slate_blue() noexcept;
					static const rgba_color& slate_gray() noexcept;
					static const rgba_color& slate_grey() noexcept;
					static const rgba_color& snow() noexcept;
					static const rgba_color& spring_green() noexcept;
					static const rgba_color& steel_blue() noexcept;
					static const rgba_color& tan() noexcept;
					static const rgba_color& teal() noexcept;
					static const rgba_color& thistle() noexcept;
					static const rgba_color& tomato() noexcept;
					static const rgba_color& transparent_black() noexcept; // Note: Not in CSS3.
					static const rgba_color& turquoise() noexcept;
					static const rgba_color& violet() noexcept;
					static const rgba_color& wheat() noexcept;
					static const rgba_color& white() noexcept;
					static const rgba_color& white_smoke() noexcept;
					static const rgba_color& yellow() noexcept;
					static const rgba_color& yellow_green() noexcept;
				};

				inline constexpr bool operator==(const rgba_color& lhs, const rgba_color& rhs) {
					return lhs.r() == rhs.r() && lhs.g() == rhs.g() && lhs.b() == rhs.b() && lhs.a() == rhs.a();
				}
				inline constexpr bool operator!=(const rgba_color& lhs, const rgba_color& rhs) {
					return !(lhs == rhs);
				}

				inline constexpr rgba_color operator*(const rgba_color& lhs, double rhs) {
					rhs = ::std::max(::std::min(rhs, 1.0), 0.0);
					return{
						::std::min(lhs.r() * rhs, 1.0),
						::std::min(lhs.g() * rhs, 1.0),
						::std::min(lhs.b() * rhs, 1.0),
						::std::min(lhs.a() * rhs, 1.0)
					};
				}

				inline constexpr rgba_color operator*(double lhs, const rgba_color& rhs) {
					lhs = ::std::max(::std::min(lhs, 1.0), 0.0);
					return{
						::std::min(lhs * rhs.r(), 1.0),
						::std::min(lhs * rhs.g(), 1.0),
						::std::min(lhs * rhs.b(), 1.0),
						::std::min(lhs * rhs.a(), 1.0)
					};
				}


#if _Inline_namespace_conditional_support_test && _User_defined_literal_conditional_support_test
				inline namespace literals {
					// Note: The _ prefix is added because certain compilers reject attempts to add a non-user-defined literal
					inline double operator""_ubyte(unsigned long long value) noexcept {
						return ::std::max(0.0, ::std::min(1.0, static_cast<double>(value) / 255.0));
					}

					// Note: The _ prefix is added because certain compilers reject attempts to add a non-user-defined literal
					inline double operator "" _unorm(long double value) noexcept {
						auto result = ::std::max(0.0, ::std::min(1.0, static_cast<double>(value)));
						result = ::std::nearbyint(result * 255.0); // We need to ensure it is one of the discrete values between 0 and 255.
						return result / 255.0;
					}
				}
#endif

				class matrix_2d {
					double _M00 = 1.0;
					double _M01 = 0.0;
					double _M10 = 0.0;
					double _M11 = 1.0;
					double _M20 = 0.0;
					double _M21 = 0.0;
				public:

					constexpr matrix_2d() noexcept { }
					constexpr matrix_2d(double m00, double m01, double m10, double m11, double m20, double m21) noexcept {
						_M00 = m00;
						_M01 = m01;
						_M10 = m10;
						_M11 = m11;
						_M20 = m20;
						_M21 = m21;
					}
					constexpr matrix_2d(const matrix_2d& other) noexcept = default;
					constexpr matrix_2d& operator=(const matrix_2d& other) noexcept = default;
					matrix_2d(matrix_2d&& other) noexcept = default;
					matrix_2d& operator=(matrix_2d&& other) noexcept = default;

					constexpr static matrix_2d init_identity() noexcept {
						return{ 1.0, 0.0, 0.0, 1.0, 0.0, 0.0 };
					}
					constexpr static matrix_2d init_translate(const vector_2d& value) noexcept {
						return{ 1.0, 0.0, 0.0, 1.0, value.x(), value.y() };
					}
					constexpr static matrix_2d init_scale(const vector_2d& value) noexcept {
						return{ value.x(), 0.0, 0.0, value.y(), 0.0, 0.0 };
					}
					static matrix_2d init_rotate(double radians) noexcept;
					constexpr static matrix_2d init_shear_x(double factor) noexcept {
						return{ 1.0, 0.0, factor, 1.0, 0.0, 0.0 };
					}
					constexpr static matrix_2d init_shear_y(double factor) noexcept {
						return{ 1.0, factor, 0.0, 1.0, 0.0, 0.0 };
					}

					// Modifiers
					void m00(double value) noexcept;
					void m01(double value) noexcept;
					void m10(double value) noexcept;
					void m11(double value) noexcept;
					void m20(double value) noexcept;
					void m21(double value) noexcept;
					matrix_2d& translate(const vector_2d& value) noexcept;
					matrix_2d& scale(const vector_2d& value) noexcept;
					matrix_2d& rotate(double radians) noexcept;
					matrix_2d& shear_x(double factor) noexcept;
					matrix_2d& shear_y(double factor) noexcept;
					matrix_2d& invert();
					matrix_2d& invert(::std::error_code& ec) noexcept;
					void swap(matrix_2d& other);

					// Observers
					constexpr double m00() const noexcept {
						return _M00;
					}
					constexpr double m01() const noexcept {
						return _M01;
					}
					constexpr double m10() const noexcept {
						return _M10;
					}
					constexpr double m11() const noexcept {
						return _M11;
					}
					constexpr double m20() const noexcept {
						return _M20;
					}
					constexpr double m21() const noexcept {
						return _M21;
					}
					constexpr bool is_invertible() const noexcept {
						return (_M00 * _M11 - _M01 * _M10) != 0.0;
					}
				private:
					constexpr bool _Is_finite_check(double val) const noexcept {
						return val != numeric_limits<double>::infinity() &&
							val != -numeric_limits<double>::infinity() &&
							val != val; // This checks for both types of NaN. Compilers should not optimize this away but the only way to be sure is to check the documentation and any compiler switches you may be using.
					}
				public:
					constexpr bool is_finite() const noexcept {
						static_assert(numeric_limits<double>::is_iec559 == true, "This implementation relies on IEEE 754 floating point behavior.");
						return numeric_limits<double>::is_iec559 &&
							_Is_finite_check(_M00) &&
							_Is_finite_check(_M01) &&
							_Is_finite_check(_M10) &&
							_Is_finite_check(_M11);
					}
					constexpr double determinant() const noexcept {
						return _M00 * _M11 - _M01 * _M10;
					}
					constexpr vector_2d transform_point(const vector_2d& pt) const noexcept {
						return{ _M00 * pt.x() + _M10 * pt.y() + _M20, _M01 * pt.x() + _M11 * pt.y() + _M21 };
					}

					constexpr vector_2d transform_distance(const vector_2d& dist) const noexcept {
						return{ _M00 * dist.x() + _M10 * dist.y(), _M01 * dist.x() + _M11 * dist.y() };
					}

					matrix_2d& operator*=(const matrix_2d& rhs) noexcept;
				};

				constexpr matrix_2d operator*(const matrix_2d& lhs, const matrix_2d& rhs) noexcept {
					return matrix_2d{
						(lhs.m00() * rhs.m00()) + (lhs.m01() * rhs.m10()),
						(lhs.m00() * rhs.m01()) + (lhs.m01() * rhs.m11()),
						(lhs.m10() * rhs.m00()) + (lhs.m11() * rhs.m10()),
						(lhs.m10() * rhs.m01()) + (lhs.m11() * rhs.m11()),
						(lhs.m20() * rhs.m00()) + (lhs.m21() * rhs.m10()) + rhs.m20(),
						(lhs.m20() * rhs.m01()) + (lhs.m21() * rhs.m11()) + rhs.m21()
					};
				}
				constexpr bool operator==(const matrix_2d& lhs, const matrix_2d& rhs) noexcept {
					return lhs.m00() == rhs.m00() && lhs.m01() == rhs.m01() &&
						lhs.m10() == rhs.m10() && lhs.m11() == rhs.m11() &&
						lhs.m20() == rhs.m20() && lhs.m21() == rhs.m21();
				}
				constexpr bool operator!=(const matrix_2d& lhs, const matrix_2d& rhs) noexcept {
					return !(lhs == rhs);
				}


				namespace path_data {
					class new_path {
					public:
						constexpr new_path() noexcept {}
						constexpr new_path(const new_path&) noexcept = default;
						constexpr new_path& operator=(const new_path&) noexcept = default;
						new_path(new_path&&) noexcept = default;
						new_path& operator=(new_path&&) noexcept = default;
					};

					class close_path {
						vector_2d _Data = {};
					public:
						constexpr explicit close_path(const vector_2d& to) noexcept
							: _Data(to) {
						}
						constexpr close_path() noexcept {}
						constexpr close_path(const close_path&) noexcept = default;
						constexpr close_path& operator=(const close_path&) noexcept = default;
						close_path(close_path&&) noexcept = default;
						close_path& operator=(close_path&&) noexcept = default;

						void to(const vector_2d& value) noexcept;
						constexpr vector_2d to() const noexcept {
							return _Data;
						}
					};

					class abs_move {
						vector_2d _Data = {};
					public:
						constexpr explicit abs_move(const vector_2d& to) noexcept
							: _Data(to) {
						}
						constexpr abs_move() noexcept {}
						constexpr abs_move(const abs_move&) noexcept = default;
						constexpr abs_move& operator=(const abs_move&) noexcept = default;
						abs_move(abs_move&&) noexcept = default;
						abs_move& operator=(abs_move&&) noexcept = default;

						void to(const vector_2d& value) noexcept;
						constexpr vector_2d to() const noexcept {
							return _Data;
						}
					};

					class abs_line {
						vector_2d _Data = {};
					public:
						constexpr explicit abs_line(const vector_2d& to) noexcept
							: _Data(to) {
						}
						constexpr abs_line() noexcept {}
						constexpr abs_line(const abs_line&) noexcept = default;
						constexpr abs_line& operator=(const abs_line&) noexcept = default;
						abs_line(abs_line&&) noexcept = default;
						abs_line& operator=(abs_line&&) noexcept = default;

						void to(const vector_2d& value) noexcept;
						constexpr vector_2d to() const noexcept {
							return _Data;
						}
					};

					class rel_move {
						vector_2d _Data = {};
					public:
						constexpr explicit rel_move(const vector_2d& to) noexcept
							: _Data(to) {
						}
						constexpr rel_move() noexcept {}
						constexpr rel_move(const rel_move&) noexcept = default;
						constexpr rel_move& operator=(const rel_move&) noexcept = default;
						rel_move(rel_move&&) noexcept = default;
						rel_move& operator=(rel_move&&) noexcept = default;

						void to(const vector_2d& value) noexcept;
						constexpr vector_2d to() const noexcept {
							return _Data;
						}
					};

					class rel_line {
						vector_2d _Data = {};
					public:
						constexpr explicit rel_line(const vector_2d& to) noexcept
							: _Data(to) {
						}
						constexpr rel_line() noexcept {}
						constexpr rel_line(const rel_line&) noexcept = default;
						constexpr rel_line& operator=(const rel_line&) noexcept = default;
						rel_line(rel_line&&) noexcept = default;
						rel_line& operator=(rel_line&&) noexcept = default;

						void to(const vector_2d& value) noexcept;
						constexpr vector_2d to() const noexcept {
							return _Data;
						}
					};

					class abs_cubic_curve {
						vector_2d _Control_pt1 = {};
						vector_2d _Control_pt2 = {};
						vector_2d _End_pt = {};
					public:
						constexpr abs_cubic_curve(const vector_2d& controlPoint1, const vector_2d& controlPoint2, const vector_2d& endPoint) noexcept
							: _Control_pt1(controlPoint1)
							, _Control_pt2(controlPoint2)
							, _End_pt(endPoint) {
						}
						constexpr abs_cubic_curve() noexcept {}
						constexpr abs_cubic_curve(const abs_cubic_curve&) noexcept = default;
						constexpr abs_cubic_curve& operator=(const abs_cubic_curve&) noexcept = default;
						abs_cubic_curve(abs_cubic_curve&&) noexcept = default;
						abs_cubic_curve& operator=(abs_cubic_curve&&) noexcept = default;

						void control_point_1(const vector_2d& value) noexcept;
						void control_point_2(const vector_2d& value) noexcept;
						void end_point(const vector_2d& value) noexcept;

						constexpr vector_2d control_point_1() const noexcept {
							return _Control_pt1;
						}
						constexpr vector_2d control_point_2() const noexcept {
							return _Control_pt2;
						}
						constexpr vector_2d end_point() const noexcept {
							return _End_pt;
						}
					};

					class rel_cubic_curve {
						vector_2d _Control_pt1 = {};
						vector_2d _Control_pt2 = {};
						vector_2d _End_pt = {};
					public:
						constexpr rel_cubic_curve(const vector_2d& controlPoint1, const vector_2d& controlPoint2, const vector_2d& endPoint) noexcept
							: _Control_pt1(controlPoint1)
							, _Control_pt2(controlPoint2)
							, _End_pt(endPoint) {
						}
						constexpr rel_cubic_curve() noexcept {}
						constexpr rel_cubic_curve(const rel_cubic_curve&) noexcept = default;
						constexpr rel_cubic_curve& operator=(const rel_cubic_curve&) noexcept = default;
						rel_cubic_curve(rel_cubic_curve&&) noexcept = default;
						rel_cubic_curve& operator=(rel_cubic_curve&&) noexcept = default;

						void control_point_1(const vector_2d& value) noexcept;
						void control_point_2(const vector_2d& value) noexcept;
						void end_point(const vector_2d& value) noexcept;

						constexpr vector_2d control_point_1() const noexcept {
							return _Control_pt1;
						}
						constexpr vector_2d control_point_2() const noexcept {
							return _Control_pt2;
						}
						constexpr vector_2d end_point() const noexcept {
							return _End_pt;
						}
					};

					class abs_quadratic_curve {
						vector_2d _Control_pt = {};
						vector_2d _End_pt = {};
					public:
						constexpr abs_quadratic_curve(const vector_2d& cp, const vector_2d& ep) noexcept
							: _Control_pt(cp)
							, _End_pt(ep) {
						}
						constexpr abs_quadratic_curve() noexcept {}
						constexpr abs_quadratic_curve(const abs_quadratic_curve&) noexcept = default;
						constexpr abs_quadratic_curve& operator=(const abs_quadratic_curve&) noexcept = default;
						abs_quadratic_curve(abs_quadratic_curve&&) noexcept = default;
						abs_quadratic_curve& operator=(abs_quadratic_curve&&) noexcept = default;

						void control_point(const vector_2d& cp) noexcept;
						void end_point(const vector_2d& ep) noexcept;

						constexpr vector_2d control_point() const noexcept {
							return _Control_pt;
						}
						constexpr vector_2d end_point() const noexcept {
							return _End_pt;
						}
					};

					class rel_quadratic_curve {
						vector_2d _Control_pt = {};
						vector_2d _End_pt = {};
					public:
						constexpr rel_quadratic_curve(const vector_2d& cp, const vector_2d& ep) noexcept
							: _Control_pt(cp)
							, _End_pt(ep) {
						}
						constexpr rel_quadratic_curve() noexcept {}
						constexpr rel_quadratic_curve(const rel_quadratic_curve&) noexcept = default;
						constexpr rel_quadratic_curve& operator=(const rel_quadratic_curve&) noexcept = default;
						rel_quadratic_curve(rel_quadratic_curve&&) noexcept = default;
						rel_quadratic_curve& operator=(rel_quadratic_curve&&) noexcept = default;

						void control_point(const vector_2d& cp) noexcept;
						void end_point(const vector_2d& ep) noexcept;

						constexpr vector_2d control_point() const noexcept {
							return _Control_pt;
						}
						constexpr vector_2d end_point() const noexcept {
							return _End_pt;
						}
					};

					class arc_clockwise {
						experimental::io2d::circle _Circle = {};
						double _Angle_1 = 0.0;
						double _Angle_2 = 0.0;
					public:
						constexpr arc_clockwise(const experimental::io2d::circle& c, double angle1, double angle2) noexcept
							: _Circle(c)
							, _Angle_1(angle1)
							, _Angle_2(angle2) {
						}
						constexpr arc_clockwise(const vector_2d& ctr, double rad, double angle1, double angle2) noexcept
							: _Circle(ctr, rad)
							, _Angle_1(angle1)
							, _Angle_2(angle2) {
						}
						constexpr arc_clockwise() noexcept {}
						constexpr arc_clockwise(const arc_clockwise&) noexcept = default;
						constexpr arc_clockwise& operator=(const arc_clockwise&) noexcept = default;
						arc_clockwise(arc_clockwise&&) noexcept = default;
						arc_clockwise& operator=(arc_clockwise&&) noexcept = default;

						void circle(const experimental::io2d::circle& c) noexcept;
						void center(const vector_2d& ctr) noexcept;
						void radius(double rad) noexcept;
						void angle_1(double radians) noexcept;
						void angle_2(double radians) noexcept;

						constexpr experimental::io2d::circle circle() const noexcept {
							return _Circle;
						}
						constexpr vector_2d center() const noexcept {
							return _Circle.center();
						}
						constexpr double radius() const noexcept {
							return _Circle.radius();
						}
						constexpr double angle_1() const noexcept {
							return _Angle_1;
						}
						constexpr double angle_2() const noexcept {
							return _Angle_2;
						}
					};

					class arc_counterclockwise {
						experimental::io2d::circle _Circle = {};
						double _Angle_1 = 0.0;
						double _Angle_2 = 0.0;
					public:
						constexpr arc_counterclockwise(const experimental::io2d::circle& c, double angle1, double angle2) noexcept
							: _Circle(c)
							, _Angle_1(angle1)
							, _Angle_2(angle2) {
						}
						constexpr arc_counterclockwise(const vector_2d& ctr, double rad, double angle1, double angle2) noexcept
							: _Circle(ctr, rad)
							, _Angle_1(angle1)
							, _Angle_2(angle2) {
						}
						constexpr arc_counterclockwise() noexcept {}
						constexpr arc_counterclockwise(const arc_counterclockwise&) noexcept = default;
						constexpr arc_counterclockwise& operator=(const arc_counterclockwise&) noexcept = default;
						arc_counterclockwise(arc_counterclockwise&&) noexcept = default;
						arc_counterclockwise& operator=(arc_counterclockwise&&) noexcept = default;

						void circle(const experimental::io2d::circle& c) noexcept;
						void center(const vector_2d& ctr) noexcept;
						void radius(double rad) noexcept;
						void angle_1(double radians) noexcept;
						void angle_2(double radians) noexcept;

						constexpr experimental::io2d::circle circle() const noexcept {
							return _Circle;
						}
						constexpr vector_2d center() const noexcept {
							return _Circle.center();
						}
						constexpr double radius() const noexcept {
							return _Circle.radius();
						}
						constexpr double angle_1() const noexcept {
							return _Angle_1;
						}
						constexpr double angle_2() const noexcept {
							return _Angle_2;
						}
					};

					class change_matrix {
						matrix_2d _Matrix = {};
					public:
						constexpr explicit change_matrix(const matrix_2d& m) noexcept
							: _Matrix(m) {
						}
						constexpr change_matrix() noexcept {}
						constexpr change_matrix(const change_matrix&) noexcept = default;
						constexpr change_matrix& operator=(const change_matrix&) noexcept = default;
						change_matrix(change_matrix&&) noexcept = default;
						change_matrix& operator=(change_matrix&&) noexcept = default;

						void matrix(const matrix_2d& value) noexcept;
						constexpr matrix_2d matrix() const noexcept {
							return _Matrix;
						}
					};

					class change_origin {
						vector_2d _Origin = {};
					public:
						constexpr explicit change_origin(const vector_2d& origin) noexcept
							: _Origin(origin) {
						}
						constexpr change_origin() noexcept {}
						constexpr change_origin(const change_origin&) noexcept = default;
						constexpr change_origin& operator=(const change_origin&) noexcept = default;
						change_origin(change_origin&&) noexcept = default;
						change_origin& operator=(change_origin&&) noexcept = default;

						void origin(const vector_2d& value) noexcept;
						constexpr vector_2d origin() const noexcept {
							return _Origin;
						}
					};

					using path_data_types = typename ::std::variant<abs_move, abs_line, rel_move, rel_line, abs_cubic_curve, rel_cubic_curve, abs_quadratic_curve, rel_quadratic_curve, arc_clockwise, arc_counterclockwise, new_path, close_path, change_matrix, change_origin>;
				}

				// Forward declaration.
				class surface;
				template <class Allocator = allocator<path_data::path_data_types>>
				class path_factory;

				class path_group {
					::std::shared_ptr<cairo_path_t> _Cairo_path;
				public:
					using native_handle_type = cairo_path*;
					// Note: Can default construct. It will just be empty. To be useful it would need to be assigned to.
					path_group() noexcept = default;
					template <class Allocator>
					explicit path_group(const path_factory<Allocator>& p);
					template <class Allocator>
					path_group(const path_factory<Allocator>& p, std::error_code& ec) noexcept;
					path_group(const path_group&) = default;
					path_group& operator=(const path_group&) = default;
					path_group(path_group&&) = default;
					path_group& operator=(path_group&&) = default;

					native_handle_type native_handle() {
						return _Cairo_path.get();
					}
				};

				enum class _Path_data_new_path {};
				constexpr static _Path_data_new_path _Path_data_new_path_val = {};
				enum class _Path_data_close_path {};
				constexpr static _Path_data_close_path _Path_data_close_path_val = {};
				enum class _Path_data_abs_cubic_curve {};
				constexpr static _Path_data_abs_cubic_curve _Path_data_abs_cubic_curve_val = {};
				enum class _Path_data_abs_line {};
				constexpr static _Path_data_abs_line _Path_data_abs_line_val = {};
				enum class _Path_data_abs_move {};
				constexpr static _Path_data_abs_move _Path_data_abs_move_val = {};
				enum class _Path_data_abs_quadratic_curve {};
				constexpr static _Path_data_abs_quadratic_curve _Path_data_abs_quadratic_curve_val = {};
				enum class _Path_data_rel_cubic_curve {};
				constexpr static _Path_data_rel_cubic_curve _Path_data_rel_cubic_curve_val = {};
				enum class _Path_data_rel_line {};
				constexpr static _Path_data_rel_line _Path_data_rel_line_val = {};
				enum class _Path_data_rel_move {};
				constexpr static _Path_data_rel_move _Path_data_rel_move_val = {};
				enum class _Path_data_rel_quadratic_curve {};
				constexpr static _Path_data_rel_quadratic_curve _Path_data_rel_quadratic_curve_val = {};
				enum class _Path_data_arc_clockwise {};
				constexpr static _Path_data_arc_clockwise _Path_data_arc_clockwise_val = {};
				enum class _Path_data_arc_counterclockwise {};
				constexpr static _Path_data_arc_counterclockwise _Path_data_arc_counterclockwise_val = {};
				enum class _Path_data_change_matrix {};
				constexpr static _Path_data_change_matrix _Path_data_change_matrix_val = {};
				enum class _Path_data_change_origin {};
				constexpr static _Path_data_change_origin _Path_data_change_origin_val = {};

				template <class _TItem>
				struct _Path_group_perform_visit {
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::abs_move>, _Path_data_abs_move> = _Path_data_abs_move_val>
					constexpr static void _Perform(::std::vector<cairo_path_data_t>& vec, const path_data::abs_move& item) noexcept {
						cairo_path_data_t cpdItem{};
						auto pt = item.to();
						cpdItem.header.type = CAIRO_PATH_MOVE_TO;
						cpdItem.header.length = 2;
						vec.push_back(cpdItem);
						cpdItem = {};
						cpdItem.point = { pt.x(), pt.y() };
						vec.push_back(cpdItem);
					}

					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::abs_line>, _Path_data_abs_line> = _Path_data_abs_line_val>
					constexpr static void _Perform(::std::vector<cairo_path_data_t>& vec, const path_data::abs_line& item) noexcept {
						cairo_path_data_t cpdItem{};
						auto pt = item.to();
						cpdItem.header.type = CAIRO_PATH_LINE_TO;
						cpdItem.header.length = 2;
						vec.push_back(cpdItem);
						cpdItem = {};
						cpdItem.point = { pt.x(), pt.y() };
						vec.push_back(cpdItem);
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::abs_cubic_curve>, _Path_data_abs_cubic_curve> = _Path_data_abs_cubic_curve_val>
					constexpr static void _Perform(::std::vector<cairo_path_data_t>& vec, const path_data::abs_cubic_curve& item) noexcept {
						cairo_path_data_t cpdItem{};
						auto pt1 = item.control_point_1();
						auto pt2 = item.control_point_2();
						auto pt3 = item.end_point();
						cpdItem.header.type = CAIRO_PATH_CURVE_TO;
						cpdItem.header.length = 4;
						vec.push_back(cpdItem);
						cpdItem = {};
						cpdItem.point = { pt1.x(), pt1.y() };
						vec.push_back(cpdItem);
						cpdItem = {};
						cpdItem.point = { pt2.x(), pt2.y() };
						vec.push_back(cpdItem);
						cpdItem = {};
						cpdItem.point = { pt3.x(), pt3.y() };
						vec.push_back(cpdItem);
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::abs_quadratic_curve>, _Path_data_abs_quadratic_curve> = _Path_data_abs_quadratic_curve_val>
					constexpr static void _Perform(::std::vector<cairo_path_data_t>&, const path_data::abs_quadratic_curve&) noexcept {
						assert(false && "Quadratic curves should have been transformed into cubic curves already.");
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::new_path>, _Path_data_new_path> = _Path_data_new_path_val>
					constexpr static void _Perform(::std::vector<cairo_path_data_t>&, const path_data::new_path&) noexcept {
						assert(false && "New path instructions should have been eliminated.");
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::close_path>, _Path_data_close_path> = _Path_data_close_path_val>
					constexpr static void _Perform(::std::vector<cairo_path_data_t>& vec, const path_data::close_path& item) noexcept {
						auto lastMoveToPoint = item.to();
						cairo_path_data_t cpdItem{};
						cpdItem.header.type = CAIRO_PATH_CLOSE_PATH;
						cpdItem.header.length = 1;
						vec.push_back(cpdItem);
						cpdItem.header.type = CAIRO_PATH_MOVE_TO;
						cpdItem.header.length = 2;
						vec.push_back(cpdItem);
						cpdItem = {};
						cpdItem.point = { lastMoveToPoint.x(), lastMoveToPoint.y() };
						vec.push_back(cpdItem);
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::rel_move>, _Path_data_rel_move> = _Path_data_rel_move_val>
					constexpr static void _Perform(::std::vector<cairo_path_data_t>&, const path_data::rel_move&) noexcept {
						assert(false && "Rel move should have been transformed into non-relative.");
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::rel_line>, _Path_data_rel_line> = _Path_data_rel_line_val>
					constexpr static void _Perform(::std::vector<cairo_path_data_t>&, const path_data::rel_line&) noexcept {
						assert(false && "Rel line should have been transformed into non-relative.");
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::rel_cubic_curve>, _Path_data_rel_cubic_curve> = _Path_data_rel_cubic_curve_val>
					constexpr static void _Perform(::std::vector<cairo_path_data_t>&, const path_data::rel_cubic_curve&) noexcept {
						assert(false && "Rel curve should have been transformed into non-relative.");
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::rel_quadratic_curve>, _Path_data_rel_quadratic_curve> = _Path_data_rel_quadratic_curve_val>
					constexpr static void _Perform(::std::vector<cairo_path_data_t>&, const path_data::rel_quadratic_curve&) noexcept {
						assert(false && "Quadratic curves should have been transformed into cubic curves.");
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::arc_clockwise>, _Path_data_arc_clockwise> = _Path_data_arc_clockwise_val>
					constexpr static void _Perform(::std::vector<cairo_path_data_t>&, const path_data::arc_clockwise&) noexcept {
						assert(false && "Quadratic curves should have been transformed into cubic curves.");
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::arc_counterclockwise>, _Path_data_arc_counterclockwise> = _Path_data_arc_counterclockwise_val>
					constexpr static void _Perform(::std::vector<cairo_path_data_t>&, const path_data::arc_counterclockwise&) {
						assert(false && "Quadratic curves should have been transformed into cubic curves.");
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::change_matrix>, _Path_data_change_matrix> = _Path_data_change_matrix_val>
					constexpr static void _Perform(::std::vector<cairo_path_data_t>&, const path_data::change_matrix&) noexcept {
						assert(false && "Change matrix should have been eliminated.");
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::change_origin>, _Path_data_change_origin> = _Path_data_change_origin_val>
					constexpr static void _Perform(::std::vector<cairo_path_data_t>&, const path_data::change_origin&) noexcept {
						assert(false && "Change origin should have been eliminated.");
					}
				};

				template <class Allocator>
				inline path_group::path_group(const path_factory<Allocator>& pf)
					: _Cairo_path(new cairo_path_t, [](cairo_path_t* path) {
					if (path != nullptr) {
						if (path->data != nullptr) {
							delete[] path->data;
							path->data = nullptr;
							path->status = CAIRO_STATUS_NULL_POINTER;
						}
						delete path;
						path = nullptr;
					}
				}) {
					auto processedVec = _Process_path_data<Allocator>(pf);
					::std::vector<cairo_path_data_t> vec;

					for (const auto& val : processedVec) {
						::std::visit([&vec](auto&& item) {
							using T = ::std::remove_cv_t<::std::remove_reference_t<decltype(item)>>;
							_Path_group_perform_visit<T>::template _Perform<T>(vec, item);
						}, val);
					}
					_Cairo_path->num_data = static_cast<int>(vec.size());
					const auto numDataST = vec.size();
					_Cairo_path->data = new cairo_path_data_t[numDataST];
					for (size_t currItemIndex = 0; currItemIndex < numDataST; currItemIndex++) {
						_Cairo_path->data[currItemIndex] = vec[currItemIndex];
					}
					_Cairo_path->status = CAIRO_STATUS_SUCCESS;
				}

				template<class Allocator>
				inline path_group::path_group(const path_factory<Allocator>& pf, ::std::error_code& ec) noexcept {
					: _Cairo_path(new cairo_path_t, [](cairo_path_t* path) {
						if (path != nullptr) {
							if (path->data != nullptr) {
								delete[] path->data;
								path->data = nullptr;
								path->status = CAIRO_STATUS_NULL_POINTER;
							}
							delete path;
							path = nullptr;
						}
					}) {
						auto processedVec = _Process_path_data<Allocator>(pf, ec);
						if (static_cast<bool>(ec)) {
							return;
						}

						::std::vector<cairo_path_data_t> vec;

						for (const auto& val : processedVec) {
							::std::visit([&vec](auto&& item) {
								using T = ::std::remove_cv_t<::std::remove_reference_t<decltype(item)>>;
								_Path_group_perform_visit<T>::_Perform<T>(vec, item);
							}, val);
						}
						_Cairo_path->num_data = static_cast<int>(vec.size());
						const auto numDataST = vec.size();
						_Cairo_path->data = new cairo_path_data_t[numDataST];
						for (size_t currItemIndex = 0; currItemIndex < numDataST; currItemIndex++) {
							_Cairo_path->data[currItemIndex] = vec[currItemIndex];
						}
						_Cairo_path->status = CAIRO_STATUS_SUCCESS;
					}
					ec.clear();
				}

				template <class Allocator>
				class path_factory {
					::std::vector<path_data::path_data_types, Allocator> _Data;
					optional<vector_2d> _Current_point;
					vector_2d _Last_move_to_point;
					matrix_2d _Transform_matrix;
					vector_2d _Origin;
				public:
					using value_type = path_data::path_data_types;
					using allocator_type = Allocator;
					using reference = value_type&;
					using const_reference = const value_type&;
					using size_type = ::std::vector<path_data::path_data_types>::size_type;
					using difference_type = ::std::vector<path_data::path_data_types>::difference_type;
					using iterator = ::std::vector<path_data::path_data_types>::iterator;
					using const_iterator = ::std::vector<path_data::path_data_types>::const_iterator;
					using reverse_iterator = std::reverse_iterator<iterator>;
					using const_reverse_iterator = std::reverse_iterator<const_iterator>;

					path_factory() noexcept(noexcept(Allocator())) :
						path_factory(Allocator()) { }
					explicit path_factory(const Allocator& a) noexcept;
					explicit path_factory(size_type n, const Allocator& a = Allocator());
					path_factory(size_type n, const value_type& value,
						const Allocator& a = Allocator());
					template <class InputIterator>
					path_factory(InputIterator first, InputIterator last,
						const Allocator& = Allocator());
					path_factory(const path_factory& pf);
					path_factory(path_factory&& pf) noexcept;
					path_factory(const path_factory& pf, const Allocator& a);
					path_factory(path_factory&& pf, const Allocator& a);
					path_factory(initializer_list<value_type> il, const Allocator& a = Allocator());
					~path_factory() { }
					path_factory& operator=(const path_factory& x);
					path_factory& operator=(path_factory&& x)
						noexcept(
							allocator_traits<Allocator>::propagate_on_container_move_assignment::value
							||
							allocator_traits<Allocator>::is_always_equal::value);
					path_factory& operator=(initializer_list<value_type> il);
					template <class InputIterator>
					void assign(InputIterator first, InputIterator last);
					void assign(size_type n, const value_type& u);
					void assign(initializer_list<value_type> il);
					allocator_type get_allocator() const noexcept;

					iterator begin() noexcept;
					const_iterator begin() const noexcept;
					const_iterator cbegin() const noexcept;
					iterator end() noexcept;
					const_iterator end() const noexcept;
					const_iterator cend() const noexcept;
					reverse_iterator rbegin() noexcept;
					const_reverse_iterator rbegin() const noexcept;
					const_reverse_iterator crbegin() const noexcept;
					reverse_iterator rend() noexcept;
					const_reverse_iterator rend() const noexcept;
					const_reverse_iterator crend() const noexcept;

					bool empty() const noexcept;
					size_type size() const noexcept;
					size_type max_size() const noexcept;
					size_type capacity() const noexcept;
					void resize(size_type sz);
					void resize(size_type sz, const value_type& c);
					void reserve(size_type n);
					void shrink_to_fit();

					reference operator[](size_type n);
					const_reference operator[](size_type n) const;
					const_reference at(size_type n) const;
					reference at(size_type n);
					reference front();
					const_reference front() const;
					reference back();
					const_reference back() const;

					// Modifiers
					void new_path() noexcept;
					void close_path() noexcept;
					void arc_clockwise(const vector_2d& center, double radius, double angle1,
						double angle2) noexcept;
					void arc_clockwise(const circle& c, double angle1,
						double angle2) noexcept;
					void arc_counterclockwise(const vector_2d& center, double radius,
						double angle1, double angle2) noexcept;
					void arc_counterclockwise(const circle& c,
						double angle1, double angle2) noexcept;
					void ellipse(const vector_2d& ctr, double width, double height) noexcept;
					void cubic_curve_to(const vector_2d& pt0, const vector_2d& pt1,
						const vector_2d& pt2) noexcept;
					void line_to(const vector_2d& pt) noexcept;
					void move_to(const vector_2d& pt) noexcept;
					void quadratic_curve_to(const vector_2d& pt0, const vector_2d& pt1)
						noexcept;
					void rectangle(const experimental::io2d::rectangle& r) noexcept;
					void rel_cubic_curve_to(const vector_2d& dpt0, const vector_2d& dpt1,
						const vector_2d& dpt2) noexcept;
					void rel_line_to(const vector_2d& dpt) noexcept;
					void rel_move_to(const vector_2d& dpt) noexcept;
					void rel_quadratic_curve_to(const vector_2d& dpt0, const vector_2d& dpt1)
						noexcept;
					void transform_matrix(const matrix_2d& m) noexcept;
					void origin(const vector_2d& pt) noexcept;
					template <class... Args>
					reference emplace_back(Args&&... args);
					void push_back(const value_type& x);
					void push_back(value_type&& x);
					void pop_back();

					template <class... Args>
					iterator emplace(const_iterator position, Args&&... args);
					iterator insert(const_iterator position, const value_type& x);
					iterator insert(const_iterator position, value_type&& x);
					iterator insert(const_iterator position, size_type n, const value_type& x);
					template <class InputIterator>
					iterator insert(const_iterator position, InputIterator first,
						InputIterator last);
					iterator insert(const_iterator position,
						initializer_list<value_type> il);
					iterator erase(const_iterator position);
					iterator erase(const_iterator first, const_iterator last);
					void swap(path_factory&)
						noexcept(allocator_traits<Allocator>::propagate_on_container_swap::value
							|| allocator_traits<Allocator>::is_always_equal::value);
					void clear() noexcept;

					// Observers
					::std::experimental::io2d::rectangle path_extents() const;
					::std::experimental::io2d::rectangle path_extents(::std::error_code& ec) const noexcept;
					//bool has_current_point() const noexcept;
					//vector_2d current_point() const;
					//vector_2d current_point(::std::error_code& ec) const noexcept;
					//matrix_2d current_matrix() const noexcept;
					//vector_2d current_origin() const noexcept;
					//::std::vector<path_data_item> data() const;
					//::std::vector<path_data_item> data(::std::error_code& ec) const noexcept;
					//path_data_item data_item(unsigned int index) const;
					//path_data_item data_item(unsigned int index, ::std::error_code& ec) const noexcept;
					//const ::std::vector<path_data_item>& data_ref() const noexcept;
				};

				template <class Allocator>
				bool operator==(const path_factory<Allocator>& lhs,
					const path_factory<Allocator>& rhs);
				template <class Allocator>
				bool operator!=(const path_factory<Allocator>& lhs,
					const path_factory<Allocator>& rhs);
				// 8.17.8, specialized algorithms:
				template <class Allocator>
				void swap(path_factory<Allocator>& lhs, path_factory<Allocator>& rhs)
					noexcept(noexcept(lhs.swap(rhs)));

				class device {
				public:
					typedef cairo_device_t* native_handle_type;
				private:
					friend surface;
					explicit device(native_handle_type nh);
					device(native_handle_type nh, error_code& ec) noexcept;
				protected:
					::std::shared_ptr<cairo_device_t> _Device;
				public:
					native_handle_type native_handle() const noexcept;

					device() = delete;
					device(const device&) = delete;
					device& operator=(const device&) = delete;
					device(device&& other) noexcept = default;
					device& operator=(device&& other) noexcept = default;

					// Modifiers
					void flush() noexcept;
					void lock();
					void lock(::std::error_code& ec) noexcept;
					void unlock();
					void unlock(::std::error_code& ec) noexcept;
				};

				class color_stop {
				private:
					double _Offset = 0.0;
					rgba_color _Color = rgba_color::black();
				public:
					color_stop() noexcept = default;
					color_stop(double offset, const rgba_color& color);

					void offset(double value) noexcept;
					void color(const rgba_color& value) noexcept;

					double offset() const noexcept;
					rgba_color color() const noexcept;
				};

				template <class Allocator = allocator<color_stop>>
				class color_stop_group {
					::std::vector<color_stop> _Stops;
				public:
					using value_type = color_stop;
					using allocator_type = Allocator;
					using pointer = typename allocator_traits<Allocator>::pointer;
					using const_pointer = typename allocator_traits<Allocator>::const_pointer;
					using reference = value_type&;
					using const_reference = const value_type&;
					using size_type = typename ::std::vector<color_stop, Allocator>::size_type;
					using difference_type = typename ::std::vector<color_stop, Allocator>::difference_type;
					using iterator = typename ::std::vector<color_stop, Allocator>::iterator;
					using const_iterator = typename ::std::vector<color_stop, Allocator>::const_iterator;
					using reverse_iterator = typename ::std::reverse_iterator<iterator>;
					using const_reverse_iterator = typename ::std::reverse_iterator<const_iterator>;

					color_stop_group() noexcept(noexcept(Allocator())) :
						color_stop_group(Allocator()) { }
					explicit color_stop_group(const Allocator& a) noexcept;
					explicit color_stop_group(size_type n, const Allocator& a = Allocator());
					color_stop_group(size_type n, const value_type& value,
						const Allocator& a = Allocator());
					template <class InputIterator>
					color_stop_group(InputIterator first, InputIterator last,
						const Allocator& a = Allocator());
					color_stop_group(const color_stop_group& x);
					color_stop_group(color_stop_group&& x) noexcept;
					color_stop_group(const color_stop_group& x, const Allocator& a);
					color_stop_group(color_stop_group&& x, const Allocator& a);
					color_stop_group(initializer_list<value_type> il,
						const Allocator& a = Allocator());
					~color_stop_group();
					color_stop_group& operator=(const color_stop_group& x);
					color_stop_group& operator=(color_stop_group&& x)
						noexcept(
							allocator_traits<Allocator>::propagate_on_container_move_assignment::value
							|| allocator_traits<Allocator>::is_always_equal::value);
					color_stop_group& operator=(initializer_list<value_type> il);
					template <class InputIterator>
					void assign(InputIterator first, InputIterator last);
					void assign(size_type n, const value_type& u);
					void assign(initializer_list<value_type> il);
					allocator_type get_allocator() const noexcept;

					// 9.5.7, iterators:
					iterator begin() noexcept;
					const_iterator begin() const noexcept;
					const_iterator cbegin() const noexcept;
					iterator end() noexcept;
					const_iterator end() const noexcept;
					const_iterator cend() const noexcept;
					reverse_iterator rbegin() noexcept;
					const_reverse_iterator rbegin() const noexcept;
					const_reverse_iterator crbegin() const noexcept;
					reverse_iterator rend() noexcept;
					const_reverse_iterator rend() const noexcept;
					const_reverse_iterator crend() const noexcept;

					bool empty() const noexcept;
					size_type size() const noexcept;
					size_type max_size() const noexcept;
					size_type capacity() const noexcept;
					void resize(size_type sz);
					void resize(size_type sz, const value_type& c);
					void reserve(size_type n);
					void shrink_to_fit();

					// element access:
					reference operator[](size_type n);
					const_reference operator[](size_type n) const;
					const_reference at(size_type n) const;
					reference at(size_type n);
					reference front();
					const_reference front() const;
					reference back();
					const_reference back() const;

					template <class... Args>
					reference emplace_back(Args&&... args);
					void push_back(const value_type& x);
					void push_back(value_type&& x);
					void pop_back();
					template <class... Args>
					iterator emplace(const_iterator position, Args&&... args);
					iterator insert(const_iterator position, const value_type& x);
					iterator insert(const_iterator position, value_type&& x);
					iterator insert(const_iterator position, size_type n, const value_type& x);
					template <class InputIterator>
					iterator insert(const_iterator position, InputIterator first,
						InputIterator last);
					iterator insert(const_iterator position,
						initializer_list<value_type> il);
					iterator erase(const_iterator position);
					iterator erase(const_iterator first, const_iterator last);
					void swap(color_stop_group& other)
						noexcept(allocator_traits<Allocator>::propagate_on_container_swap::value
							|| allocator_traits<Allocator>::is_always_equal::value);
					void clear() noexcept;
				};

				template <class Allocator>
				bool operator==(const color_stop_group<Allocator>& lhs,
					const color_stop_group<Allocator>& rhs);
				template <class Allocator>
				bool operator!=(const color_stop_group<Allocator>& lhs,
					const color_stop_group<Allocator>& rhs);
				// 8.17.8, specialized algorithms:
				template <class Allocator>
				void swap(color_stop_group<Allocator>& lhs, color_stop_group<Allocator>& rhs)
					noexcept(noexcept(lhs.swap(rhs))) {
					lhs.swap(rhs);
				}

				//template <class... _Args>
				//void _Find_first_variadic_type();
				//template <class T, class... _Args>
				//T _Find_first_variadic_type();
				//template <class T>
				//T _Find_first_variadic_type();
				//template<>
				//void _Find_first_variadic_type();

				class surface;
				class image_surface;
				class display_surface;

				//template <class _T, class... _Args>
				//void _Add_color_stop(cairo_pattern_t*, _T&&, _Args&&...);

				//template <class _T>
				//void _Add_color_stop(cairo_pattern_t*, _T&&);

				//template <>
				//void _Add_color_stop(cairo_pattern_t*);

				//template <class _T, class... _Args>
				//inline void _Add_color_stop(cairo_pattern_t* pat, _T&& stop, _Args&&... rest) {
				//	cairo_pattern_add_color_stop_rgba(pat, stop.offset(), stop.color().r(), stop.color().g(), stop.color().b(), stop.color().a());
				//	if (cairo_pattern_status(pat) != CAIRO_STATUS_SUCCESS) {
				//		return;
				//	}
				//	_Add_color_stop<_Args&&...>(pat, ::std::forward<_Args>(rest)...);
				//}

				//template <class _T>
				//inline void _Add_color_stop(cairo_pattern_t* pat, _T&& stop) {
				//	cairo_pattern_add_color_stop_rgba(pat, stop.offset(), stop.color().r(), stop.color().g(), stop.color().b(), stop.color().a());
				//}

				//template <>
				//inline void _Add_color_stop(cairo_pattern_t*) {
				//}

				class brush {
				public:
					typedef cairo_pattern_t* native_handle_type;

				private:
					friend surface;
					friend display_surface;
					// Precondition: nh has already had its reference count incremented (either in creation or with cairo_pattern_reference).
					brush(native_handle_type nh) noexcept;

					::std::shared_ptr<cairo_pattern_t> _Brush;
					::std::shared_ptr<image_surface> _Image_surface;
					brush_type _Brush_type;
					::std::experimental::io2d::tiling _Extend;
					::std::experimental::io2d::filter _Filter;
					matrix_2d _Matrix;

				public:
					native_handle_type native_handle() const noexcept;

					brush() = delete;
					brush(const brush&) noexcept = default;
					brush& operator=(const brush&) noexcept = default;
					brush(brush&& other) noexcept = default;
					brush& operator=(brush&& other) noexcept = default;
					brush(const rgba_color& c);
					brush(const rgba_color& c, error_code& ec) noexcept;
					template <class _Allocator>
					brush(const vector_2d& begin, const vector_2d& end,
						const color_stop_group<_Allocator>& csg);
					template <class _Allocator>
					brush(const vector_2d& begin, const vector_2d& end,
						const color_stop_group<_Allocator>& csg, error_code& ec) noexcept;

					brush(const vector_2d& begin, const vector_2d& end,
						::std::initializer_list<color_stop> il);

					brush(const vector_2d& begin, const vector_2d& end, ::std::error_code& ec,
						::std::initializer_list<color_stop> il) noexcept;

					template <class _Allocator>
					brush(const circle& start, const circle& end,
						const color_stop_group<_Allocator>& csg);
					template <class _Allocator>
					brush(const circle& start, const circle& end,
						const color_stop_group<_Allocator>& csg, error_code& ec) noexcept;

					brush(const circle& start, const circle& end,
						::std::initializer_list<color_stop> il);

					brush(const circle& start, const circle& end, ::std::error_code& ec,
						::std::initializer_list<color_stop> il) noexcept;

					brush(image_surface&& img);
					brush(image_surface&& img, error_code& ec) noexcept;

					void tiling(::std::experimental::io2d::tiling e) noexcept;
					void filter(::std::experimental::io2d::filter f) noexcept;
					void matrix(const matrix_2d& m) noexcept;

					::std::experimental::io2d::tiling tiling() const noexcept;
					::std::experimental::io2d::filter filter() const noexcept;
					matrix_2d matrix() const noexcept;
					brush_type type() const noexcept;
					//const image_surface& surface() const;
					//const image_surface& surface(::std::error_code& ec) const noexcept;

				};

				struct _Surface_native_handles {
					::cairo_surface_t* csfce;
					::cairo_t* cctxt;
				};

				class mapped_surface;

				// tuple<dashes, offset>
				typedef ::std::tuple<::std::vector<double>, double> dashes;

				// I don't know why Clang/C2 is complaining about weak vtables here since the at least one virtual function is always anchored but for now silence the warnings. I've never seen this using Clang on OpenSUSE.
#ifdef _WIN32
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif
#endif

				class surface {
				public:
					typedef _Surface_native_handles native_handle_type;
				private:
					::std::weak_ptr<::std::experimental::io2d::device> _Device;
				protected:
					::std::unique_ptr<cairo_surface_t, decltype(&cairo_surface_destroy)> _Surface;
					::std::unique_ptr<cairo_t, decltype(&cairo_destroy)> _Context;
					::std::unique_ptr<cairo_font_options_t, decltype(&cairo_font_options_destroy)> _Native_font_options;

					const double _Line_join_miter_miter_limit = 10000.0;

					// State - unsaved
					typedef rectangle _Dirty_type;
					_Dirty_type _Dirty_rect;
					::std::experimental::io2d::format _Format;
					::std::experimental::io2d::content _Content;

					// State - saved
					::std::experimental::io2d::brush _Brush;
					::std::experimental::io2d::antialias _Antialias;
					::std::experimental::io2d::dashes _Dashes;
					::std::experimental::io2d::fill_rule _Fill_rule;
					::std::experimental::io2d::line_cap _Line_cap;
					::std::experimental::io2d::line_join _Line_join = ::std::experimental::io2d::line_join::miter;
					typedef double _Line_width_type;
					_Line_width_type _Line_width;
					typedef double _Miter_limit_type;
					_Miter_limit_type _Miter_limit = 10.0;
					::std::experimental::io2d::compositing_operator _Compositing_operator;
					::std::shared_ptr<::std::experimental::io2d::path_group> _Current_path;
					::std::experimental::io2d::path_factory<> _Immediate_path;
					typedef matrix_2d _Transform_matrix_type;
					_Transform_matrix_type _Transform_matrix;

					// Relies on C++17 noexcept guarantee for vector default ctor (N4258, adopted 2014-11).
					::std::stack < ::std::tuple<
						::std::experimental::io2d::brush,
						::std::experimental::io2d::antialias,
						::std::experimental::io2d::dashes,
						::std::experimental::io2d::fill_rule,
						::std::experimental::io2d::line_cap,
						::std::experimental::io2d::line_join,
						_Line_width_type,
						_Miter_limit_type,
						::std::experimental::io2d::compositing_operator,
						::std::shared_ptr<::std::experimental::io2d::path_group>,
						::std::experimental::io2d::path_factory<>,
						_Transform_matrix_type
					>, ::std::vector<::std::tuple<
						::std::experimental::io2d::brush,
						::std::experimental::io2d::antialias,
						::std::experimental::io2d::dashes,
						::std::experimental::io2d::fill_rule,
						::std::experimental::io2d::line_cap,
						::std::experimental::io2d::line_join,
						_Line_width_type,
						_Miter_limit_type,
						::std::experimental::io2d::compositing_operator,
						::std::shared_ptr<::std::experimental::io2d::path_group>,
						::std::experimental::io2d::path_factory<>,
						_Transform_matrix_type>> > _Saved_state;

					void _Ensure_state();
					void _Ensure_state(::std::error_code& ec) noexcept;

					surface(::std::experimental::io2d::format fmt, int width, int height);
					surface(::std::experimental::io2d::format fmt, int width, int height, ::std::error_code& ec) noexcept;

					surface(native_handle_type nh, ::std::experimental::io2d::format fmt, ::std::experimental::io2d::content ctnt);
					surface(native_handle_type nh, ::std::experimental::io2d::format fmt, ::std::experimental::io2d::content ctnt, ::std::error_code& ec) noexcept;

					// create_similar
					surface(const surface& other, ::std::experimental::io2d::content ctnt, int width, int height);
					surface(const surface& other, ::std::experimental::io2d::content ctnt, int width, int height, ::std::error_code& ec) noexcept;

					void path_group(const ::std::shared_ptr<::std::experimental::io2d::path_group>& p);
					void path_group(const ::std::shared_ptr<::std::experimental::io2d::path_group>& p, ::std::error_code& ec) noexcept;
				public:
					bool _Has_surface_resource() const noexcept;
					native_handle_type native_handle() const;

					surface() = delete;
					surface(const surface&) = delete;
					surface& operator=(const surface&) = delete;
					surface(surface&& other) /*noexcept*/ = default;
					surface& operator=(surface&& other) /*noexcept*/ = default;

					virtual ~surface();

					// \ref{\iotwod.surface.modifiers.state}, state modifiers:
					virtual void finish() noexcept;
					void flush();
					void flush(::std::error_code& ec) noexcept;
					::std::shared_ptr<::std::experimental::io2d::device> device();
					::std::shared_ptr<::std::experimental::io2d::device> device(::std::error_code& ec) noexcept;
					void mark_dirty();
					void mark_dirty(::std::error_code& ec) noexcept;
					void mark_dirty(const rectangle& rect);
					void mark_dirty(const rectangle& rect, ::std::error_code& ec) noexcept;
					void map(const ::std::function<void(mapped_surface&)>& action);
					void map(const ::std::function<void(mapped_surface&, error_code&)>& action, ::std::error_code& ec);
					void map(const ::std::function<void(mapped_surface&)>& action, const rectangle& extents);
					void map(const ::std::function<void(mapped_surface&, error_code&)>& action, const rectangle& extents, ::std::error_code& ec);
					virtual void save();
					virtual void save(::std::error_code& ec) noexcept;
					virtual void restore();
					virtual void restore(::std::error_code& ec) noexcept;
					void brush(experimental::io2d::nullvalue_t) noexcept;
					void brush(const ::std::experimental::io2d::brush& source);
					void brush(const ::std::experimental::io2d::brush& source, ::std::error_code& ec) noexcept;
					void antialias(::std::experimental::io2d::antialias a) noexcept;
					void dashes(experimental::io2d::nullvalue_t) noexcept;
					void dashes(const ::std::experimental::io2d::dashes& d);
					void dashes(const ::std::experimental::io2d::dashes& d, ::std::error_code& ec) noexcept;
					void fill_rule(::std::experimental::io2d::fill_rule fr) noexcept;
					void line_cap(::std::experimental::io2d::line_cap lc) noexcept;
					void line_join(::std::experimental::io2d::line_join lj) noexcept;
					void line_width(double width) noexcept;
					void miter_limit(double limit) noexcept;
					void compositing_operator(::std::experimental::io2d::compositing_operator co) noexcept;
					//void clip(experimental::nullopt_t) noexcept;
					void clip(const ::std::experimental::io2d::path_group& p);
					void clip(const ::std::experimental::io2d::path_group& p, ::std::error_code& ec) noexcept;
					void clip_immediate();
					void clip_immediate(::std::error_code& ec) noexcept;
					void path_group(experimental::io2d::nullvalue_t) noexcept;
					void path_group(const ::std::experimental::io2d::path_group& p);
					void path_group(const ::std::experimental::io2d::path_group& p, ::std::error_code& ec) noexcept;

					// \ref{\iotwod.surface.modifiers.immediatepath}, immediate path_group modifiers:
					::std::experimental::io2d::path_factory<>& immediate() noexcept;

					// \ref{\iotwod.surface.modifiers.render}, render modifiers:
					void clear();
					void fill();
					void fill(::std::error_code& ec) noexcept;
					void fill(const rgba_color& c);
					void fill(const rgba_color& c, ::std::error_code& ec) noexcept;
					void fill(const ::std::experimental::io2d::brush& b);
					void fill(const ::std::experimental::io2d::brush& b, ::std::error_code& ec) noexcept;
					void fill(const surface& s, const matrix_2d& m = matrix_2d::init_identity(), tiling e = tiling::none, filter f = filter::good);
					void fill(const surface& s, ::std::error_code& ec, const matrix_2d& m = matrix_2d::init_identity(), tiling e = tiling::none, filter f = filter::good) noexcept;
					void fill_immediate();
					void fill_immediate(::std::error_code& ec) noexcept;
					void fill_immediate(const rgba_color& c);
					void fill_immediate(const rgba_color& c, ::std::error_code& ec) noexcept;
					void fill_immediate(const ::std::experimental::io2d::brush& b);
					void fill_immediate(const ::std::experimental::io2d::brush& b, ::std::error_code& ec) noexcept;
					void fill_immediate(const surface& s, const matrix_2d& m = matrix_2d::init_identity(), tiling e = tiling::none, filter f = filter::good);
					void fill_immediate(const surface& s, ::std::error_code& ec, const matrix_2d& m = matrix_2d::init_identity(), tiling e = tiling::none, filter f = filter::good) noexcept;
					void paint();
					void paint(::std::error_code& ec) noexcept;
					void paint(const rgba_color& c);
					void paint(const rgba_color& c, ::std::error_code& ec) noexcept;
					void paint(const ::std::experimental::io2d::brush& b);
					void paint(const ::std::experimental::io2d::brush& b, ::std::error_code& ec) noexcept;
					void paint(const surface& s, const matrix_2d& m = matrix_2d::init_identity(), tiling e = tiling::none, filter f = filter::good);
					void paint(const surface& s, ::std::error_code& ec, const matrix_2d& m = matrix_2d::init_identity(), tiling e = tiling::none, filter f = filter::good) noexcept;
					void paint(double alpha);
					void paint(double alpha, ::std::error_code& ec) noexcept;
					void paint(const rgba_color& c, double alpha);
					void paint(const rgba_color& c, double alpha, ::std::error_code& ec) noexcept;
					void paint(const ::std::experimental::io2d::brush& b, double alpha);
					void paint(const ::std::experimental::io2d::brush& b, double alpha, ::std::error_code& ec) noexcept;
					void paint(const surface& s, double alpha, const matrix_2d& m = matrix_2d::init_identity(), tiling e = tiling::none, filter f = filter::good);
					void paint(const surface& s, double alpha, ::std::error_code& ec, const matrix_2d& m = matrix_2d::init_identity(), tiling e = tiling::none, filter f = filter::good) noexcept;
					void stroke();
					void stroke(::std::error_code& ec) noexcept;
					void stroke(const rgba_color& c);
					void stroke(const rgba_color& c, ::std::error_code& ec) noexcept;
					void stroke(const ::std::experimental::io2d::brush& b);
					void stroke(const ::std::experimental::io2d::brush& b, ::std::error_code& ec) noexcept;
					void stroke(const surface& s, const matrix_2d& m = matrix_2d::init_identity(), tiling e = tiling::none, filter f = filter::good);
					void stroke(const surface& s, ::std::error_code& ec, const matrix_2d& m = matrix_2d::init_identity(), tiling e = tiling::none, filter f = filter::good) noexcept;
					void stroke_immediate();
					void stroke_immediate(::std::error_code& ec) noexcept;
					void stroke_immediate(const rgba_color& c);
					void stroke_immediate(const rgba_color& c, ::std::error_code& ec) noexcept;
					void stroke_immediate(const ::std::experimental::io2d::brush& b);
					void stroke_immediate(const ::std::experimental::io2d::brush& b, ::std::error_code& ec) noexcept;
					void stroke_immediate(const surface& s, const matrix_2d& m = matrix_2d::init_identity(), tiling e = tiling::none, filter f = filter::good);
					void stroke_immediate(const surface& s, ::std::error_code& ec, const matrix_2d& m = matrix_2d::init_identity(), tiling e = tiling::none, filter f = filter::good) noexcept;

					// \ref{\iotwod.surface.modifiers.maskrender}, mask render modifiers:
					void mask(const ::std::experimental::io2d::brush& maskBrush);
					void mask(const ::std::experimental::io2d::brush& maskBrush, ::std::error_code& ec) noexcept;
					void mask(const ::std::experimental::io2d::brush& maskBrush, const rgba_color& c);
					void mask(const ::std::experimental::io2d::brush& maskBrush, const rgba_color& c, ::std::error_code& ec) noexcept;
					void mask(const ::std::experimental::io2d::brush& maskBrush, const ::std::experimental::io2d::brush& b);
					void mask(const ::std::experimental::io2d::brush& maskBrush, const ::std::experimental::io2d::brush& b, ::std::error_code& ec) noexcept;
					void mask(const ::std::experimental::io2d::brush& maskBrush, const surface& s, const matrix_2d& m = matrix_2d::init_identity(), tiling e = tiling::none, filter f = filter::good);
					void mask(const ::std::experimental::io2d::brush& maskBrush, const surface& s, ::std::error_code& ec, const matrix_2d& m = matrix_2d::init_identity(), tiling e = tiling::none, filter f = filter::good) noexcept;
					void mask_immediate(const ::std::experimental::io2d::brush& maskBrush);
					void mask_immediate(const ::std::experimental::io2d::brush& maskBrush, ::std::error_code& ec) noexcept;
					void mask_immediate(const ::std::experimental::io2d::brush& maskBrush, const rgba_color& c);
					void mask_immediate(const ::std::experimental::io2d::brush& maskBrush, const rgba_color& c, ::std::error_code& ec) noexcept;
					void mask_immediate(const ::std::experimental::io2d::brush& maskBrush, const ::std::experimental::io2d::brush& b);
					void mask_immediate(const ::std::experimental::io2d::brush& maskBrush, const ::std::experimental::io2d::brush& b, ::std::error_code& ec) noexcept;
					void mask_immediate(const ::std::experimental::io2d::brush& maskBrush, const surface& s, const matrix_2d& m = matrix_2d::init_identity(), tiling e = tiling::none, filter f = filter::good);
					void mask_immediate(const ::std::experimental::io2d::brush& maskBrush, const surface& s, ::std::error_code& ec, const matrix_2d& m = matrix_2d::init_identity(), tiling e = tiling::none, filter f = filter::good) noexcept;

					// \ref{\iotwod.surface.modifiers.transform}, transformation modifiers:
					void matrix(const matrix_2d& matrix);
					void matrix(const matrix_2d& matrix, ::std::error_code& ec) noexcept;

					// \ref{\iotwod.surface.observers.state}, state observers:
					bool is_finished() const noexcept;
					::std::experimental::io2d::content content() const noexcept;
					::std::experimental::io2d::brush brush() const noexcept;
					::std::experimental::io2d::antialias antialias() const noexcept;
					::std::experimental::io2d::dashes dashes() const;
					::std::experimental::io2d::dashes dashes(::std::error_code& ec) const noexcept;
					::std::experimental::io2d::fill_rule fill_rule() const noexcept;
					::std::experimental::io2d::line_cap line_cap() const noexcept;
					::std::experimental::io2d::line_join line_join() const noexcept;
					double line_width() const noexcept;
					double miter_limit() const noexcept;
					::std::experimental::io2d::compositing_operator compositing_operator() const noexcept;
					rectangle clip_extents() const noexcept;
					bool in_clip(const vector_2d& pt) const noexcept;
					::std::vector<rectangle> clip_rectangles() const;
					::std::vector<rectangle> clip_rectangles(::std::error_code& ec) const noexcept;

					// \ref{\iotwod.surface.observers.render}, render observers:
					rectangle fill_extents() const noexcept;
					rectangle fill_extents_immediate() const noexcept;
					bool in_fill(const vector_2d& pt) const noexcept;
					bool in_fill_immediate(const vector_2d& pt) const noexcept;
					rectangle stroke_extents() const noexcept;
					rectangle stroke_extents_immediate() const noexcept;
					bool in_stroke(const vector_2d& pt) const noexcept;
					bool in_stroke_immediate(const vector_2d& pt) const noexcept;

					// \ref{\iotwod.surface.observers.transform}, transformation observers:
					matrix_2d matrix() const noexcept;
					vector_2d user_to_surface(const vector_2d& pt) const noexcept;
					vector_2d surface_to_user(const vector_2d& pt) const noexcept;
				};

				// I don't know why Clang/C2 is complaining about weak vtables here since the at least one virtual function is always anchored but for now silence the warnings. I've never seen this using Clang on OpenSUSE.
#ifdef _WIN32
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#endif

				// I don't know why Clang/C2 is complaining about weak vtables here since the at least one virtual function is always anchored but for now silence the warnings. I've never seen this using Clang on OpenSUSE.
#ifdef _WIN32
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif
#endif
				class image_surface : public surface {
					friend surface;
				public:
					image_surface() = delete;
					image_surface(const image_surface&) = delete;
					image_surface& operator=(const image_surface&) = delete;
					image_surface(image_surface&& other) /*noexcept*/ = default;
					image_surface& operator=(image_surface&& other) /*noexcept*/ = default;
					image_surface(::std::experimental::io2d::format fmt, int width, int height);
					image_surface(::std::experimental::io2d::format fmt, int width, int height, ::std::error_code& ec) noexcept;
					template <class InputIt>
					image_surface(InputIt first, InputIt last, ::std::experimental::io2d::format fmt, int width, int height);
					template <class InputIt>
					image_surface(InputIt first, InputIt last, ::std::experimental::io2d::format fmt, int width, int height, ::std::error_code& ec) noexcept;
					//// create_similar_image
					//image_surface(const surface& other, ::std::experimental::io2d::format fmt, int width, int height);
					//image_surface(const surface& other, ::std::experimental::io2d::format fmt, int width, int height, ::std::error_code& ec) noexcept;
					//// create_from_png
					//image_surface(const ::std::string& filename);
					virtual ~image_surface() { }

					// Modifiers
					void data(const ::std::vector<unsigned char>& data);
					void data(const ::std::vector<unsigned char>& data, ::std::error_code& ec) noexcept;
					::std::vector<unsigned char> data();
					::std::vector<unsigned char> data(::std::error_code& ec) noexcept;

					// Observers
					::std::experimental::io2d::format format() const noexcept;
					int width() const noexcept;
					int height() const noexcept;
					int stride() const noexcept;
				};

				// I don't know why Clang/C2 is complaining about weak vtables here since the at least one virtual function is always anchored but for now silence the warnings. I've never seen this using Clang on OpenSUSE.
#ifdef _WIN32
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#endif

				class mapped_surface {
					surface::native_handle_type _Mapped_surface;
					surface::native_handle_type _Map_of;

					friend surface;
					mapped_surface(surface::native_handle_type nh, surface::native_handle_type map_of);
					mapped_surface(surface::native_handle_type nh, surface::native_handle_type map_of, ::std::error_code& ec) noexcept;

				public:
					mapped_surface() = delete;
					mapped_surface(const mapped_surface&) = delete;
					mapped_surface& operator=(const mapped_surface&) = delete;
					mapped_surface(mapped_surface&& other) = delete;
					mapped_surface& operator=(mapped_surface&& other) = delete;
					~mapped_surface();

					// Modifiers
					void commit_changes();
					void commit_changes(::std::error_code& ec) noexcept;
					void commit_changes(const rectangle& area);
					void commit_changes(const rectangle& area, ::std::error_code& ec) noexcept;

					unsigned char* data();
					unsigned char* data(::std::error_code& ec) noexcept;
					//void rgba_data(const ::std::vector<rgba_color>& v);
					//void rgba_data(const ::std::vector<rgba_color>& v, ::std::error_code& ec) noexcept;
					//void rgba_data(const ::std::vector<rgba_color>& v, unsigned int offset);
					//void rgba_data(const ::std::vector<rgba_color>& v, unsigned int offset, ::std::error_code& ec) noexcept;

					// Observers
					const unsigned char* data() const;
					const unsigned char* data(::std::error_code& ec) const noexcept;
					//::std::vector<rgba_color> rgba_data() const;
					//::std::vector<rgba_color> rgba_data(::std::error_code& ec) const noexcept;

					::std::experimental::io2d::format format() const noexcept;
					int width() const noexcept;
					int height() const noexcept;
					int stride() const noexcept;
				};

#ifdef _WIN32_WINNT
				struct _Win32_display_surface_native_handle {
					_Surface_native_handles sfc_nh;
					HWND hwnd;
					_Surface_native_handles win32_sfc_nh;
				};

				const int _Display_surface_ptr_window_data_byte_offset = 0;

				LRESULT CALLBACK _RefImplWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
#elif defined(USE_XCB)
				struct _Xcb_display_surface_native_handle {
					_Surface_native_handles sfc_nh;
					_Surface_native_handles display_sfc_nh;
					xcb_connection_t* connection;
					::std::mutex& connection_mutex;
					int& connection_ref_count;
					xcb_screen_t* screen;
					xcb_window_t wndw;
				};
#elif defined(USE_XLIB)
				struct _Xlib_display_surface_native_handle {
					_Surface_native_handles sfc_nh;
					Display* display;
					Window wndw;
					::std::mutex& display_mutex;
					int& display_ref_count;
				};
#endif

				// I don't know why Clang/C2 is complaining about weak vtables here since the at least one virtual function is always anchored but for now silence the warnings. I've never seen this using Clang on OpenSUSE.
#ifdef _WIN32
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif
#endif
				class display_surface : public surface {
					friend surface;
					// Unsaved state.
					::std::experimental::io2d::brush _Default_brush;
					typedef int _Display_width_type;
					_Display_width_type _Display_width;
					typedef int _Display_height_type;
					_Display_height_type _Display_height;
					::std::experimental::io2d::scaling _Scaling;
					typedef int _Width_type;
					_Width_type _Width;
					typedef int _Height_type;
					_Height_type _Height;
					::std::function<void(display_surface& sfc)> _Draw_fn;
					::std::function<void(display_surface& sfc)> _Size_change_fn;
					typedef ::std::function<::std::experimental::io2d::rectangle(const display_surface&, bool&)> _User_scaling_fn_type;
					_User_scaling_fn_type _User_scaling_fn;
					::std::experimental::io2d::brush _Letterbox_brush;
					typedef bool _Auto_clear_type;
					_Auto_clear_type _Auto_clear;

					//::std::stack<::std::tuple<
					//	::std::experimental::io2d::scaling,
					//	_Width_type,
					//	_Height_type,
					//	_User_scaling_fn_type,
					//	::std::experimental::io2d::brush
					//>, ::std::vector<::std::tuple<
					//	::std::experimental::io2d::scaling,
					//	_Width_type,
					//	_Height_type,
					//	_User_scaling_fn_type,
					//	::std::experimental::io2d::brush>>> _Display_saved_state;
#ifdef _WIN32_WINNT
					friend LRESULT CALLBACK _RefImplWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
					DWORD _Window_style;
					HWND _Hwnd;

					LRESULT _Window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
#elif defined(USE_XCB)
					static ::std::mutex _Connection_mutex;
					static ::std::unique_ptr<xcb_connection_t, decltype(&xcb_disconnect)> _Connection;
					static int _Connection_ref_count;
					xcb_screen_t* _Screen;
					xcb_window_t _Wndw;
					bool _Can_draw = false;
					const uint16_t _Window_border_width = 4;
#elif defined(USE_XLIB)
					static ::std::mutex _Display_mutex;
					static ::std::unique_ptr<Display, ::std::function<void(Display*)>> _Display;
					static int _Display_ref_count;
					::Window _Wndw;
					bool _Can_draw = false;

					static Bool _X11_if_event_pred(Display* display, XEvent* event, XPointer arg);
#endif
					::std::experimental::io2d::refresh_rate _Refresh_rate;
					double _Desired_frame_rate;
					::std::atomic<bool> _Redraw_requested;
					double _Elapsed_draw_time;
					const double _Minimum_frame_rate = 0.01;
					const double _Maximum_frame_rate = 120.0;
					::std::unique_ptr<cairo_surface_t, ::std::function<void(cairo_surface_t*)>> _Native_surface;
					::std::unique_ptr<cairo_t, ::std::function<void(cairo_t*)>> _Native_context;

					void _Make_native_surface_and_context();
					void _Make_native_surface_and_context(::std::error_code& ec) noexcept;
					void _All_dimensions(int w, int h, int dw, int dh);
					void _All_dimensions(int w, int h, int dw, int dh, ::std::error_code& ec) noexcept;
					void _Render_to_native_surface();
					void _Render_to_native_surface(::std::error_code& ec) noexcept;
					void _Resize_window();
					void _Resize_window(::std::error_code& ec) noexcept;
					void _Render_for_scaling_uniform_or_letterbox();
					void _Render_for_scaling_uniform_or_letterbox(::std::error_code& ec) noexcept;

				public:
#ifdef _WIN32_WINNT
					typedef _Win32_display_surface_native_handle native_handle_type;
#elif defined(USE_XCB)
					typedef _Xcb_display_surface_native_handle native_handle_type;
#elif defined(USE_XLIB)
					typedef _Xlib_display_surface_native_handle native_handle_type;
#endif
					native_handle_type native_handle() const;

					display_surface() = delete;
					display_surface(const display_surface&) = delete;
					display_surface& operator=(const display_surface&) = delete;
					display_surface(display_surface&& other) noexcept;
					display_surface& operator=(display_surface&& other) noexcept;

					display_surface(int preferredWidth, int preferredHeight, ::std::experimental::io2d::format preferredFormat,
						::std::experimental::io2d::scaling scl = ::std::experimental::io2d::scaling::letterbox, ::std::experimental::io2d::refresh_rate rr = ::std::experimental::io2d::refresh_rate::as_fast_as_possible, double fps = 30.0);
					display_surface(int preferredWidth, int preferredHeight, ::std::experimental::io2d::format preferredFormat, ::std::error_code& ec,
						::std::experimental::io2d::scaling scl = ::std::experimental::io2d::scaling::letterbox, ::std::experimental::io2d::refresh_rate rr = ::std::experimental::io2d::refresh_rate::as_fast_as_possible, double fps = 30.0) noexcept;

					display_surface(int preferredWidth, int preferredHeight, ::std::experimental::io2d::format preferredFormat,
						int preferredDisplayWidth, int preferredDisplayHeight,
						::std::experimental::io2d::scaling scl = ::std::experimental::io2d::scaling::letterbox, ::std::experimental::io2d::refresh_rate rr = ::std::experimental::io2d::refresh_rate::as_fast_as_possible, double fps = 30.0);
					display_surface(int preferredWidth, int preferredHeight, ::std::experimental::io2d::format preferredFormat,
						int preferredDisplayWidth, int preferredDisplayHeight, ::std::error_code& ec,
						::std::experimental::io2d::scaling scl = ::std::experimental::io2d::scaling::letterbox, ::std::experimental::io2d::refresh_rate rr = ::std::experimental::io2d::refresh_rate::as_fast_as_possible, double fps = 30.0) noexcept;

					virtual ~display_surface();

					virtual void save() override;
					virtual void save(::std::error_code& ec) noexcept override;
					virtual void restore() override;
					virtual void restore(::std::error_code& ec) noexcept override;

					void draw_callback(const ::std::function<void(display_surface& sfc)>& fn);
					void draw_callback(const ::std::function<void(display_surface& sfc)>& fn, ::std::error_code& ec) noexcept;
					void size_change_callback(const ::std::function<void(display_surface& sfc)>& fn);
					void size_change_callback(const ::std::function<void(display_surface& sfc)>& fn, ::std::error_code& ec) noexcept;
					void width(int w);
					void width(int w, ::std::error_code& ec) noexcept;
					void height(int h);
					void height(int h, ::std::error_code& ec) noexcept;
					void display_width(int w);
					void display_width(int w, ::std::error_code& ec) noexcept;
					void display_height(int h);
					void display_height(int h, ::std::error_code& ec) noexcept;
					void dimensions(int w, int h);
					void dimensions(int w, int h, ::std::error_code& ec) noexcept;
					void display_dimensions(int dw, int dh);
					void display_dimensions(int dw, int dh, ::std::error_code& ec) noexcept;
					void scaling(::std::experimental::io2d::scaling scl) noexcept;
					void user_scaling_callback(const ::std::function<::std::experimental::io2d::rectangle(const display_surface&, bool&)>& fn);
					void user_scaling_callback(const ::std::function<::std::experimental::io2d::rectangle(const display_surface&, bool&)>& fn, ::std::error_code& ec) noexcept;
					void letterbox_brush(experimental::io2d::nullvalue_t) noexcept;
					void letterbox_brush(const rgba_color& c);
					void letterbox_brush(const rgba_color& c, ::std::error_code& ec) noexcept;
					void letterbox_brush(const ::std::experimental::io2d::brush& b);
					void letterbox_brush(const ::std::experimental::io2d::brush& b, ::std::error_code& ec) noexcept;
					void auto_clear(bool val) noexcept;
					void refresh_rate(::std::experimental::io2d::refresh_rate rr) noexcept;
					bool desired_frame_rate(double fps) noexcept;
					void redraw_required() noexcept;

					int show();
					int show(::std::error_code& ec); // Not noexcept because if the user-provided functions throw they will propagate, but otherwise is non-throwing.
					void exit_show(int ms) noexcept;

					::std::experimental::io2d::format format() const noexcept;
					int width() const noexcept;
					int height() const noexcept;
					int display_width() const noexcept;
					int display_height() const noexcept;
					::std::tuple<int, int> dimensions() const noexcept;
					::std::tuple<int, int> display_dimensions() const noexcept;
					::std::experimental::io2d::scaling scaling() const noexcept;
					::std::function<::std::experimental::io2d::rectangle(const display_surface&, bool&)> user_scaling_callback() const;
					::std::function<::std::experimental::io2d::rectangle(const display_surface&, bool&)> user_scaling_callback(::std::error_code& ec) const noexcept;
					::std::experimental::io2d::brush letterbox_brush() const noexcept;
					bool auto_clear() const noexcept;
					::std::experimental::io2d::refresh_rate refresh_rate() const noexcept;
					double desired_frame_rate() const noexcept;
					double elapsed_draw_time() const noexcept;
				};

				// I don't know why Clang/C2 is complaining about weak vtables here since the at least one virtual function is always anchored but for now silence the warnings. I've never seen this using Clang on OpenSUSE.
#ifdef _WIN32
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#endif

				int format_stride_for_width(format format, int width) noexcept;
				display_surface make_display_surface(int preferredWidth, int preferredHeight, format preferredFormat, scaling scl = scaling::letterbox, refresh_rate rr = refresh_rate::as_fast_as_possible, double desiredFramerate = 30.0);
				display_surface make_display_surface(int preferredWidth, int preferredHeight, format preferredFormat, ::std::error_code& ec, scaling scl = scaling::letterbox, refresh_rate rr = refresh_rate::as_fast_as_possible, double desiredFramerate = 30.0) noexcept;
				display_surface make_display_surface(int preferredWidth, int preferredHeight, format preferredFormat, int preferredDisplayWidth, int preferredDisplayHeight, scaling scl = scaling::letterbox, refresh_rate rr = refresh_rate::as_fast_as_possible, double desiredFramerate = 30.0);
				display_surface make_display_surface(int preferredWidth, int preferredHeight, format preferredFormat,
					int preferredDisplayWidth, int preferredDisplayHeight, ::std::error_code& ec, scaling scl = scaling::letterbox, refresh_rate rr = refresh_rate::as_fast_as_possible, double desiredFramerate = 30.0) noexcept;
				image_surface make_image_surface(format format, int width, int height);
				image_surface make_image_surface(format format, int width, int height, ::std::error_code& ec) noexcept;

				template <class _TItem>
				struct _Path_factory_process_visit {
					constexpr static double twoThirds = 2.0 / 3.0;

					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::abs_move>, _Path_data_abs_move> = _Path_data_abs_move_val>
					static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, optional<vector_2d>& currentPoint, vector_2d& closePoint) {
						currentPoint = item.to();
						auto pt = m.transform_point(currentPoint.value() - origin) + origin;
						v.emplace_back(::std::in_place_type<path_data::abs_move>, pt);
						closePoint = pt;
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::abs_line>, _Path_data_abs_line> = _Path_data_abs_line_val>
					static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, optional<vector_2d>& currentPoint, vector_2d& closePoint) {
						if (currentPoint.has_value()) {
							currentPoint = item.to();
							auto pt = m.transform_point(currentPoint.value() - origin) + origin;
							v.emplace_back(::std::in_place_type<path_data::abs_line>, pt);
						}
						else {
							currentPoint = item.to();
							auto pt = m.transform_point(currentPoint.value() - origin) + origin;
							v.emplace_back(::std::in_place_type<path_data::abs_move>, pt);
							v.emplace_back(::std::in_place_type<path_data::abs_line>, pt);
							closePoint = pt;
						}
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::abs_cubic_curve>, _Path_data_abs_cubic_curve> = _Path_data_abs_cubic_curve_val>
					static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, optional<vector_2d>& currentPoint, vector_2d& closePoint) {
						auto pt1 = m.transform_point(item.control_point_1() - origin) + origin;
						auto pt2 = m.transform_point(item.control_point_2() - origin) + origin;
						auto pt3 = m.transform_point(item.end_point() - origin) + origin;
						if (!currentPoint.has_value()) {
							currentPoint = item.control_point_1();
							v.emplace_back(::std::in_place_type<path_data::abs_move>, pt1);
							closePoint = pt1;
						}
						v.emplace_back(::std::in_place_type<path_data::abs_cubic_curve>, pt1,
							pt2, pt3);
						currentPoint = item.end_point();
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::abs_quadratic_curve>, _Path_data_abs_quadratic_curve> = _Path_data_abs_quadratic_curve_val>
					static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, optional<vector_2d>& currentPoint, vector_2d& closePoint) {
						// Turn it into a cubic curve since cairo doesn't have quadratic curves.
						vector_2d beginPt;
						auto controlPt = m.transform_point(item.control_point() - origin) + origin;
						auto endPt = m.transform_point(item.end_point() - origin) + origin;
						if (!currentPoint.has_value()) {
							currentPoint = item.control_point();
							v.emplace_back(::std::in_place_type<path_data::abs_move>, controlPt);
							closePoint = controlPt;
							beginPt = controlPt;
						}
						else {
							beginPt = m.transform_point(currentPoint.value() - origin) + origin;
						}
						vector_2d cpt1 = { ((controlPt.x() - beginPt.x()) * twoThirds) + beginPt.x(), ((controlPt.y() - beginPt.y()) * twoThirds) + beginPt.y() };
						vector_2d cpt2 = { ((controlPt.x() - endPt.x()) * twoThirds) + endPt.x(), ((controlPt.y() - endPt.y()) * twoThirds) + endPt.y() };
						v.emplace_back(::std::in_place_type<path_data::abs_cubic_curve>, cpt1, cpt2, endPt);
						currentPoint = item.end_point();
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::new_path>, _Path_data_new_path> = _Path_data_new_path_val>
					static void _Perform(const T&, ::std::vector<path_data::path_data_types>&, matrix_2d&, vector_2d&, optional<vector_2d>& currentPoint, vector_2d&) {
						currentPoint.reset();
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::close_path>, _Path_data_close_path> = _Path_data_close_path_val>
					static void _Perform(const T&, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, optional<vector_2d>& currentPoint, vector_2d& closePoint) {
						if (currentPoint.has_value()) {
							v.emplace_back(::std::in_place_type<path_data::close_path>, closePoint);
							v.emplace_back(::std::in_place_type<path_data::abs_move>,
								closePoint);
							if (!m.is_finite() || !m.is_invertible()) {
								throw ::std::system_error(::std::make_error_code(io2d_error::invalid_matrix));
							}
							auto invM = matrix_2d{ m }.invert();
							// Need to assign the untransformed closePoint value to currentPoint.
							currentPoint = invM.transform_point(closePoint - origin) + origin;
						}
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::rel_move>, _Path_data_rel_move> = _Path_data_rel_move_val>
					static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, optional<vector_2d>& currentPoint, vector_2d& closePoint) {
						if (!currentPoint.has_value()) {
							throw ::std::system_error(::std::make_error_code(io2d_error::invalid_path_data));
						}
						currentPoint = item.to() + currentPoint.value();
						auto pt = m.transform_point(currentPoint.value() - origin) + origin;
						v.emplace_back(::std::in_place_type<path_data::abs_move>, pt);
						closePoint = pt;
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::rel_line>, _Path_data_rel_line> = _Path_data_rel_line_val>
					static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, optional<vector_2d>& currentPoint, vector_2d&) {
						if (!currentPoint.has_value()) {
							throw ::std::system_error(::std::make_error_code(io2d_error::invalid_path_data));
						}
						currentPoint = item.to() + currentPoint.value();
						auto pt = m.transform_point(currentPoint.value() - origin) + origin;
						v.emplace_back(::std::in_place_type<path_data::abs_line>, pt);
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::rel_cubic_curve>, _Path_data_rel_cubic_curve> = _Path_data_rel_cubic_curve_val>
					static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, optional<vector_2d>& currentPoint, vector_2d&) {
						if (!currentPoint.has_value()) {
							throw ::std::system_error(::std::make_error_code(io2d_error::invalid_path_data));
						}
						auto pt1 = m.transform_point(item.control_point_1() + currentPoint.value() -
							origin) + origin;
						auto pt2 = m.transform_point(item.control_point_2() + currentPoint.value() -
							origin) + origin;
						auto pt3 = m.transform_point(item.end_point() + currentPoint.value() - origin) +
							origin;
						v.emplace_back(::std::in_place_type<path_data::abs_cubic_curve>,
							pt1, pt2, pt3);
						currentPoint = item.end_point() + currentPoint.value();
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::rel_quadratic_curve>, _Path_data_rel_quadratic_curve> = _Path_data_rel_quadratic_curve_val>
					static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, optional<vector_2d>& currentPoint, vector_2d&) {
						if (!currentPoint.has_value()) {
							throw ::std::system_error(::std::make_error_code(io2d_error::invalid_path_data));
						}
						// Turn it into a cubic curve since cairo doesn't have quadratic curves.
						vector_2d beginPt;
						auto controlPt = m.transform_point(item.control_point() + currentPoint.value() -
							origin) + origin;
						auto endPt = m.transform_point(item.end_point() + currentPoint.value() -
							origin) + origin;
						beginPt = m.transform_point(currentPoint.value() - origin) + origin;
						vector_2d cpt1 = { ((controlPt.x() - beginPt.x()) * twoThirds) + beginPt.x(), ((controlPt.y() - beginPt.y()) * twoThirds) + beginPt.y() };
						vector_2d cpt2 = { ((controlPt.x() - endPt.x()) * twoThirds) + endPt.x(), ((controlPt.y() - endPt.y()) * twoThirds) + endPt.y() };
						v.emplace_back(::std::in_place_type<path_data::abs_cubic_curve>, cpt1, cpt2, endPt);
						currentPoint = item.end_point() + currentPoint.value();
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::arc_clockwise>, _Path_data_arc_clockwise> = _Path_data_arc_clockwise_val>
					static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, optional<vector_2d>& currentPoint, vector_2d& closePoint) {
						{
							auto ctr = item.center();
							auto rad = item.radius();
							auto ang1 = item.angle_1();
							auto ang2 = item.angle_2();
							while (ang2 < ang1) {
								ang2 += two_pi<double>;
							}
							vector_2d pt0, pt1, pt2, pt3;
							int bezCount = 1;
							double theta = ang2 - ang1;
							double phi{};
#if _Variable_templates_conditional_support_test
							while (theta >= half_pi<double>) {
#else
							while (theta >= half_pi<double>()) {
#endif
								theta /= 2.0;
								bezCount += bezCount;
							}
							phi = theta / 2.0;
							auto cosPhi = cos(phi);
							auto sinPhi = sin(phi);
							pt0.x(cosPhi);
							pt0.y(-sinPhi);
							pt3.x(pt0.x());
							pt3.y(-pt0.y());
							pt1.x((4.0 - cosPhi) / 3.0);
							pt1.y(-(((1.0 - cosPhi) * (3.0 - cosPhi)) / (3.0 * sinPhi)));
							pt2.x(pt1.x());
							pt2.y(-pt1.y());
							phi = -phi;
							auto rotCwFn = [](const vector_2d& pt, double a) -> vector_2d {
								return { pt.x() * cos(a) + pt.y() * sin(a),
									-(pt.x() * -(sin(a)) + pt.y() * cos(a)) };
							};
							pt0 = rotCwFn(pt0, phi);
							pt1 = rotCwFn(pt1, phi);
							pt2 = rotCwFn(pt2, phi);
							pt3 = rotCwFn(pt3, phi);

							auto currTheta = ang1;
							const auto startPt =
								ctr + rotCwFn({ pt0.x() * rad, pt0.y() * rad }, currTheta);
							if (currentPoint.has_value()) {
								currentPoint = startPt;
								auto pt = m.transform_point(currentPoint.value() - origin) + origin;
								v.emplace_back(::std::in_place_type<path_data::abs_line>, pt);
							}
							else {
								currentPoint = startPt;
								auto pt = m.transform_point(currentPoint.value() - origin) + origin;
								v.emplace_back(::std::in_place_type<path_data::abs_move>, pt);
								closePoint = pt;
							}
							for (; bezCount > 0; bezCount--) {
								auto cpt1 = ctr + rotCwFn({ pt1.x() * rad, pt1.y() * rad }, currTheta);
								auto cpt2 = ctr + rotCwFn({ pt2.x() * rad, pt2.y() * rad },
									currTheta);
								auto cpt3 = ctr + rotCwFn({ pt3.x() * rad, pt3.y() * rad },
									currTheta);
								currentPoint = cpt3;
								cpt1 = m.transform_point(cpt1 - origin) + origin;
								cpt2 = m.transform_point(cpt2 - origin) + origin;
								cpt3 = m.transform_point(cpt3 - origin) + origin;
								v.emplace_back(::std::in_place_type<path_data::abs_cubic_curve>, cpt1,
									cpt2, cpt3);
								currTheta += theta;
							}
						}
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::arc_counterclockwise>, _Path_data_arc_counterclockwise> = _Path_data_arc_counterclockwise_val>
					static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, optional<vector_2d>& currentPoint, vector_2d& closePoint) {
						{
							auto ctr = item.center();
							auto rad = item.radius();
							auto ang1 = item.angle_1();
							auto ang2 = item.angle_2();
							while (ang2 > ang1) {
								ang2 -= two_pi<double>;
							}
							vector_2d pt0, pt1, pt2, pt3;
							int bezCount = 1;
							double theta = ang1 - ang2;
							double phi{};
#if _Variable_templates_conditional_support_test
							while (theta >= half_pi<double>) {
#else
							while (theta >= half_pi<double>()) {
#endif
								theta /= 2.0;
								bezCount += bezCount;
							}
							phi = theta / 2.0;
							auto cosPhi = cos(phi);
							auto sinPhi = sin(phi);
							pt0.x(cosPhi);
							pt0.y(-sinPhi);
							pt3.x(pt0.x());
							pt3.y(-pt0.y());
							pt1.x((4.0 - cosPhi) / 3.0);
							pt1.y(-(((1.0 - cosPhi) * (3.0 - cosPhi)) / (3.0 * sinPhi)));
							pt2.x(pt1.x());
							pt2.y(-pt1.y());
							auto rotCwFn = [](const vector_2d& pt, double a) -> vector_2d {
								return { pt.x() * cos(a) + pt.y() * sin(a),
									-(pt.x() * -(sin(a)) + pt.y() * cos(a)) };
							};
							pt0 = rotCwFn(pt0, phi);
							pt1 = rotCwFn(pt1, phi);
							pt2 = rotCwFn(pt2, phi);
							pt3 = rotCwFn(pt3, phi);
							auto shflPt = pt3;
							pt3 = pt0;
							pt0 = shflPt;
							shflPt = pt2;
							pt2 = pt1;
							pt1 = shflPt;
							auto currTheta = ang1;
							const auto startPt =
								ctr + rotCwFn({ pt0.x() * rad, pt0.y() * rad }, currTheta);
							if (currentPoint.has_value()) {
								currentPoint = startPt;
								auto pt = m.transform_point(currentPoint.value() - origin) + origin;
								v.emplace_back(::std::in_place_type<path_data::abs_line>, pt);
							}
							else {
								currentPoint = startPt;
								auto pt = m.transform_point(currentPoint.value() - origin) + origin;
								v.emplace_back(::std::in_place_type<path_data::abs_move>, pt);
								closePoint = pt;
							}
							for (; bezCount > 0; bezCount--) {
								auto cpt1 = ctr + rotCwFn({ pt1.x() * rad, pt1.y() * rad },
									currTheta);
								auto cpt2 = ctr + rotCwFn({ pt2.x() * rad, pt2.y() * rad },
									currTheta);
								auto cpt3 = ctr + rotCwFn({ pt3.x() * rad, pt3.y() * rad },
									currTheta);
								currentPoint = cpt3;
								cpt1 = m.transform_point(cpt1 - origin) + origin;
								cpt2 = m.transform_point(cpt2 - origin) + origin;
								cpt3 = m.transform_point(cpt3 - origin) + origin;
								v.emplace_back(::std::in_place_type<path_data::abs_cubic_curve>, cpt1,
									cpt2, cpt3);
								currTheta -= theta;
							}
						}
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::change_matrix>, _Path_data_change_matrix> = _Path_data_change_matrix_val>
					static void _Perform(const T& item, ::std::vector<path_data::path_data_types>&, matrix_2d& m, vector_2d&, optional<vector_2d>&, vector_2d&) {
						if (!m.is_finite()) {
							throw ::std::system_error(::std::make_error_code(io2d_error::invalid_matrix));
						}
						if (!m.is_invertible()) {
							throw ::std::system_error(::std::make_error_code(io2d_error::invalid_matrix));
							return;
						}
						m = item.matrix();
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::change_origin>, _Path_data_change_origin> = _Path_data_change_origin_val>
					static void _Perform(const T& item, ::std::vector<path_data::path_data_types>&, matrix_2d&, vector_2d& origin, optional<vector_2d>&, vector_2d&) {
						origin = item.origin();
					}
				};

				template <class Allocator>
				inline ::std::vector<path_data::path_data_types> _Process_path_data(const path_factory<Allocator>& pf) {
					matrix_2d m;
					vector_2d origin;
					optional<vector_2d> currentPoint = optional<vector_2d>{}; // Tracks the untransformed current point.
					vector_2d closePoint;   // Tracks the transformed close point.
					::std::vector<path_data::path_data_types> v;

					for (const path_data::path_data_types& val : pf) {
						::std::visit([&m, &origin, &currentPoint, &closePoint, &v](auto&& item) {
							using T = ::std::remove_cv_t<::std::remove_reference_t<decltype(item)>>;
							_Path_factory_process_visit<T>::template _Perform<T>(item, v, m, origin, currentPoint, closePoint);
						}, val);
					}
					return v;
				}

				template <class _TItem>
				struct _Path_factory_process_visit_noexcept {
					constexpr static double twoThirds = 2.0 / 3.0;

					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::abs_move>, _Path_data_abs_move> = _Path_data_abs_move_val>
					constexpr static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, vector_2d& currentPoint, bool& hasCurrentPoint, vector_2d& closePoint, ::std::error_code&) noexcept {
						currentPoint = item.to();
						auto pt = m.transform_point(currentPoint - origin) + origin;
						hasCurrentPoint = true;
						v.emplace_back(::std::in_place_type<path_data::abs_move>, pt);
						closePoint = pt;
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::abs_line>, _Path_data_abs_line> = _Path_data_abs_line_val>
					constexpr static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, vector_2d& currentPoint, bool& hasCurrentPoint, vector_2d& closePoint, ::std::error_code&) noexcept {
						currentPoint = item.to();
						auto pt = m.transform_point(currentPoint - origin) + origin;
						if (hasCurrentPoint) {
							v.emplace_back(::std::in_place_type<path_data::abs_line>, pt);
						}
						else {
							v.emplace_back(::std::in_place_type<path_data::abs_move>, pt);
							v.emplace_back(::std::in_place_type<path_data::abs_line>, pt);
							hasCurrentPoint = true;
							closePoint = pt;
						}
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::abs_cubic_curve>, _Path_data_abs_cubic_curve> = _Path_data_abs_cubic_curve_val>
					constexpr static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, vector_2d& currentPoint, bool& hasCurrentPoint, vector_2d& closePoint, ::std::error_code&) noexcept {
						auto pt1 = m.transform_point(item.control_point_1() - origin) + origin;
						auto pt2 = m.transform_point(item.control_point_2() - origin) + origin;
						auto pt3 = m.transform_point(item.end_point() - origin) + origin;
						if (!hasCurrentPoint) {
							currentPoint = item.control_point_1();
							v.emplace_back(::std::in_place_type<path_data::abs_move>, pt1);
							hasCurrentPoint = true;
							closePoint = pt1;
						}
						v.emplace_back(::std::in_place_type<path_data::abs_cubic_curve>, pt1,
							pt2, pt3);
						currentPoint = item.end_point();
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::abs_quadratic_curve>, _Path_data_abs_quadratic_curve> = _Path_data_abs_quadratic_curve_val>
					constexpr static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, vector_2d& currentPoint, bool& hasCurrentPoint, vector_2d& closePoint, ::std::error_code&) noexcept {
						// Turn it into a cubic curve since cairo doesn't have quadratic curves.
						vector_2d beginPt;
						auto controlPt = m.transform_point(item.control_point() - origin) + origin;
						auto endPt = m.transform_point(item.end_point() - origin) + origin;
						if (!hasCurrentPoint) {
							currentPoint = item.control_point();
							v.emplace_back(::std::in_place_type<path_data::abs_move>, controlPt);
							hasCurrentPoint = true;
							closePoint = controlPt;
							beginPt = controlPt;
						}
						else {
							beginPt = m.transform_point(currentPoint - origin) + origin;
						}
						vector_2d cpt1 = { ((controlPt.x() - beginPt.x()) * twoThirds) + beginPt.x(), ((controlPt.y - beginPt.y()) * twoThirds) + beginPt.y() };
						vector_2d cpt2 = { ((controlPt.x() - endPt.x()) * twoThirds) + endPt.x(), ((controlPt.y - endPt.y()) * twoThirds) + endPt.y() };
						v.emplace_back(::std::in_place_type<path_data::abs_cubic_curve>, cpt1, cpt2, endPt);
						currentPoint = item.end_point();
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::new_path>, _Path_data_new_path> = _Path_data_new_path_val>
					constexpr static void _Perform(const T&, ::std::vector<path_data::path_data_types>&, matrix_2d&, vector_2d&, vector_2d&, bool& hasCurrentPoint, vector_2d&, ::std::error_code&) noexcept {
						hasCurrentPoint = false;
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::close_path>, _Path_data_close_path> = _Path_data_close_path_val>
					constexpr static void _Perform(const T&, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, vector_2d& currentPoint, bool& hasCurrentPoint, vector_2d& closePoint, ::std::error_code&) noexcept {
						if (hasCurrentPoint) {
							v.emplace_back(::std::in_place_type<path_data::close_path>, closePoint);
							v.emplace_back(::std::in_place_type<path_data::abs_move>,
								closePoint);
							auto invM = matrix_2d{ m }.invert();
							// Need to assign the untransformed closePoint value to currentPoint.
							currentPoint = invM.transform_point(closePoint - origin) + origin;
						}
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::rel_move>, _Path_data_rel_move> = _Path_data_rel_move_val>
					constexpr static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, vector_2d& currentPoint, bool& hasCurrentPoint, vector_2d& closePoint, ::std::error_code& ec) noexcept {
						if (!hasCurrentPoint) {
							ec = ::std::make_error_code(io2d_error::invalid_path_data);
							return;
						}
						currentPoint = item.to() + currentPoint;
						auto pt = m.transform_point(currentPoint - origin) + origin;
						v.emplace_back(::std::in_place_type<path_data::abs_move>, pt);
						hasCurrentPoint = true;
						closePoint = pt;
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::rel_line>, _Path_data_rel_line> = _Path_data_rel_line_val>
					constexpr static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, vector_2d& currentPoint, bool& hasCurrentPoint, vector_2d&, ::std::error_code& ec) noexcept {
						if (!hasCurrentPoint) {
							ec = ::std::make_error_code(io2d_error::invalid_path_data);
							return;
						}
						currentPoint = item.to() + currentPoint;
						auto pt = m.transform_point(currentPoint - origin) + origin;
						v.emplace_back(::std::in_place_type<path_data::abs_line>, pt);
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::rel_cubic_curve>, _Path_data_rel_cubic_curve> = _Path_data_rel_cubic_curve_val>
					constexpr static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, vector_2d& currentPoint, bool& hasCurrentPoint, vector_2d&, ::std::error_code& ec) noexcept {
						if (!hasCurrentPoint) {
							ec = ::std::make_error_code(io2d_error::invalid_path_data);
							return;
						}
						auto pt1 = m.transform_point(item.control_point_1() + currentPoint -
							origin) + origin;
						auto pt2 = m.transform_point(item.control_point_2() + currentPoint -
							origin) + origin;
						auto pt3 = m.transform_point(item.end_point() + currentPoint - origin) +
							origin;
						v.emplace_back(::std::in_place_type<path_data::abs_cubic_curve>,
							pt1, pt2, pt3);
						currentPoint = item.end_point() + currentPoint;
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::rel_quadratic_curve>, _Path_data_rel_quadratic_curve> = _Path_data_rel_quadratic_curve_val>
					constexpr static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, vector_2d& currentPoint, bool& hasCurrentPoint, vector_2d&, ::std::error_code& ec) noexcept {
						if (!hasCurrentPoint) {
							ec = ::std::make_error_code(io2d_error::invalid_path_data);
							return;
						}
						// Turn it into a cubic curve since cairo doesn't have quadratic curves.
						vector_2d beginPt;
						auto controlPt = m.transform_point(item.control_point() + currentPoint -
							origin) + origin;
						auto endPt = m.transform_point(item.end_point() + currentPoint -
							origin) + origin;
						beginPt = m.transform_point(currentPoint - origin) + origin;
						vector_2d cpt1 = { ((controlPt.x() - beginPt.x()) * twoThirds) + beginPt.x(), ((controlPt.y - beginPt.y()) * twoThirds) + beginPt.y() };
						vector_2d cpt2 = { ((controlPt.x() - endPt.x()) * twoThirds) + endPt.x(), ((controlPt.y - endPt.y()) * twoThirds) + endPt.y() };
						v.emplace_back(::std::in_place_type<path_data::abs_cubic_curve>, cpt1, cpt2, endPt);
						currentPoint = item.end_point() + currentPoint;
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::arc_clockwise>, _Path_data_arc_clockwise> = _Path_data_arc_clockwise_val>
					constexpr static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, vector_2d& currentPoint, bool& hasCurrentPoint, vector_2d& closePoint, ::std::error_code&) noexcept {
						{
							auto ctr = item.center();
							auto rad = item.radius();
							auto ang1 = item.angle_1();
							auto ang2 = item.angle_2();
							while (ang2 < ang1) {
								ang2 += two_pi<double>;
							}
							vector_2d pt0, pt1, pt2, pt3;
							int bezCount = 1;
							double theta = ang2 - ang1;
							double phi{};
#if _Variable_templates_conditional_support_test
							while (theta >= half_pi<double>) {
#else
							while (theta >= half_pi<double>()) {
#endif
								theta /= 2.0;
								bezCount += bezCount;
							}
							phi = theta / 2.0;
							auto cosPhi = cos(phi);
							auto sinPhi = sin(phi);
							pt0.x(cosPhi);
							pt0.y(-sinPhi);
							pt3.x(pt0.x());
							pt3.y(-pt0.y());
							pt1.x((4.0 - cosPhi) / 3.0);
							pt1.y(-(((1.0 - cosPhi) * (3.0 - cosPhi)) / (3.0 * sinPhi)));
							pt2.x(pt1.x());
							pt2.y(-pt1.y());
							phi = -phi;
							auto rotCwFn = [](const vector_2d& pt, double a) -> vector_2d {
								return { pt.x() * cos(a) + pt.y() * sin(a),
									-(pt.x() * -(sin(a)) + pt.y() * cos(a)) };
							};
							pt0 = rotCwFn(pt0, phi);
							pt1 = rotCwFn(pt1, phi);
							pt2 = rotCwFn(pt2, phi);
							pt3 = rotCwFn(pt3, phi);

							auto currTheta = ang1;
							const auto startPt =
								ctr + rotCwFn({ pt0.x() * rad, pt0.y() * rad }, currTheta);
							if (hasCurrentPoint) {
								currentPoint = startPt;
								auto pt = m.transform_point(currentPoint - origin) + origin;
								v.emplace_back(::std::in_place_type<path_data::abs_line>, pt);
							}
							else {
								currentPoint = startPt;
								auto pt = m.transform_point(currentPoint - origin) + origin;
								v.emplace_back(::std::in_place_type<path_data::abs_move>, pt);
								hasCurrentPoint = true;
								closePoint = pt;
							}
							for (; bezCount > 0; bezCount--) {
								auto cpt1 = ctr + rotCwFn({ pt1.x() * rad, pt1.y() * rad }, currTheta);
								auto cpt2 = ctr + rotCwFn({ pt2.x() * rad, pt2.y() * rad },
									currTheta);
								auto cpt3 = ctr + rotCwFn({ pt3.x() * rad, pt3.y() * rad },
									currTheta);
								currentPoint = cpt3;
								cpt1 = m.transform_point(cpt1 - origin) + origin;
								cpt2 = m.transform_point(cpt2 - origin) + origin;
								cpt3 = m.transform_point(cpt3 - origin) + origin;
								v.emplace_back(::std::in_place_type<path_data::abs_cubic_curve>, cpt1,
									cpt2, cpt3);
								currTheta += theta;
							}
						}
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::arc_counterclockwise>, _Path_data_arc_counterclockwise> = _Path_data_arc_counterclockwise_val>
					constexpr static void _Perform(const T& item, ::std::vector<path_data::path_data_types>& v, matrix_2d& m, vector_2d& origin, vector_2d& currentPoint, bool& hasCurrentPoint, vector_2d& closePoint, ::std::error_code&) noexcept {
						{
							auto ctr = item.center();
							auto rad = item.radius();
							auto ang1 = item.angle_1();
							auto ang2 = item.angle_2();
							while (ang2 > ang1) {
								ang2 -= two_pi<double>;
							}
							vector_2d pt0, pt1, pt2, pt3;
							int bezCount = 1;
							double theta = ang1 - ang2;
							double phi{};
#if _Variable_templates_conditional_support_test
							while (theta >= half_pi<double>) {
#else
							while (theta >= half_pi<double>()) {
#endif
								theta /= 2.0;
								bezCount += bezCount;
							}
							phi = theta / 2.0;
							auto cosPhi = cos(phi);
							auto sinPhi = sin(phi);
							pt0.x(cosPhi);
							pt0.y(-sinPhi);
							pt3.x(pt0.x());
							pt3.y(-pt0.y());
							pt1.x((4.0 - cosPhi) / 3.0);
							pt1.y(-(((1.0 - cosPhi) * (3.0 - cosPhi)) / (3.0 * sinPhi)));
							pt2.x(pt1.x());
							pt2.y(-pt1.y());
							auto rotCwFn = [](const vector_2d& pt, double a) -> vector_2d {
								return { pt.x() * cos(a) + pt.y() * sin(a),
									-(pt.x() * -(sin(a)) + pt.y() * cos(a)) };
							};
							pt0 = rotCwFn(pt0, phi);
							pt1 = rotCwFn(pt1, phi);
							pt2 = rotCwFn(pt2, phi);
							pt3 = rotCwFn(pt3, phi);
							auto shflPt = pt3;
							pt3 = pt0;
							pt0 = shflPt;
							shflPt = pt2;
							pt2 = pt1;
							pt1 = shflPt;
							auto currTheta = ang1;
							const auto startPt =
								ctr + rotCwFn({ pt0.x() * rad, pt0.y() * rad }, currTheta);
							if (hasCurrentPoint) {
								currentPoint = startPt;
								auto pt = m.transform_point(currentPoint - origin) + origin;
								v.emplace_back(::std::in_place_type<path_data::abs_line>, pt);
							}
							else {
								currentPoint = startPt;
								auto pt = m.transform_point(currentPoint - origin) + origin;
								v.emplace_back(::std::in_place_type<path_data::abs_move>, pt);
								hasCurrentPoint = true;
								closePoint = pt;
							}
							for (; bezCount > 0; bezCount--) {
								auto cpt1 = ctr + rotCwFn({ pt1.x() * rad, pt1.y() * rad },
									currTheta);
								auto cpt2 = ctr + rotCwFn({ pt2.x() * rad, pt2.y() * rad },
									currTheta);
								auto cpt3 = ctr + rotCwFn({ pt3.x() * rad, pt3.y() * rad },
									currTheta);
								currentPoint = cpt3;
								cpt1 = m.transform_point(cpt1 - origin) + origin;
								cpt2 = m.transform_point(cpt2 - origin) + origin;
								cpt3 = m.transform_point(cpt3 - origin) + origin;
								v.emplace_back(::std::in_place_type<path_data::abs_cubic_curve>, cpt1,
									cpt2, cpt3);
								currTheta -= theta;
							}
						}
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::change_matrix>, _Path_data_change_matrix> = _Path_data_change_matrix_val>
					constexpr static void _Perform(const T& item, ::std::vector<path_data::path_data_types>&, matrix_2d& m, vector_2d&, vector_2d&, bool&, vector_2d&, ::std::error_code& ec) noexcept {
						if (!m.is_finite()) {
							ec = ::std::make_error_code(io2d_error::invalid_matrix);
							return;
						}
						if (!m.is_invertible()) {
							ec = ::std::make_error_code(io2d_error::invalid_matrix);
							return;
						}
						m = item.matrix();
					}
					template <class T, ::std::enable_if_t<::std::is_same_v<T, path_data::change_origin>, _Path_data_change_origin> = _Path_data_change_origin_val>
					constexpr static void _Perform(const T& item, ::std::vector<path_data::path_data_types>&, matrix_2d&, vector_2d& origin, vector_2d&, bool&, vector_2d&, ::std::error_code&) noexcept {
						origin = item.origin();
					}
				};

				template <class Allocator>
				::std::vector<path_data::path_data_types> _Process_path_data(const path_factory<Allocator>& pf, ::std::error_code& ec) {
					matrix_2d m;
					vector_2d origin;
					vector_2d currentPoint; // Tracks the untransformed current point.
					bool hasCurrentPoint = false;
					vector_2d closePoint;   // Tracks the transformed close point.
					::std::vector<path_data::path_data_types> v;

					for (const path_data::path_data_types& val : pf) {
						::std::visit([&m, &origin, &currentPoint, &hasCurrentPoint, &closePoint, &v](auto&& item) {
							using T = ::std::remove_cv_t<::std::remove_reference_t<decltype(item)>>;
							_Path_factory_process_visit_noexcept<T>::template _Perform<T>(item, v, m, origin, currentPoint, hasCurrentPoint, closePoint, ec);
						}, val);
						if (static_cast<bool>(ec)) {
							v.clear();
							return v;
						}
					}
					return v;
				}

				template<class Allocator>
				inline path_factory<Allocator>::path_factory(const Allocator &a) noexcept
					: _Data(a)
					, _Current_point()
					, _Last_move_to_point()
					, _Transform_matrix()
					, _Origin() {
				}

				template<class Allocator>
				inline path_factory<Allocator>::path_factory(size_type n, const Allocator & a)
					: _Data(n, a)
					, _Current_point()
					, _Last_move_to_point()
					, _Transform_matrix()
					, _Origin() {
				}

				template<class Allocator>
				inline path_factory<Allocator>::path_factory(size_type n, const value_type & value, const Allocator& a)
					: _Data(n, value, a)
					, _Current_point()
					, _Last_move_to_point()
					, _Transform_matrix()
					, _Origin() {
				}

				template<class Allocator>
				template<class InputIterator>
				inline path_factory<Allocator>::path_factory(InputIterator first, InputIterator last, const Allocator& a)
					: _Data(first, last, a)
					, _Current_point()
					, _Last_move_to_point()
					, _Transform_matrix()
					, _Origin() {
				}

				template<class Allocator>
				template<class InputIterator>
				inline void path_factory<Allocator>::assign(InputIterator first, InputIterator last) {
					_Data.assign(first, last);
				}

				template<class Allocator>
				template<class ...Args>
				inline typename path_factory<Allocator>::reference path_factory<Allocator>::emplace_back(Args && ...args) {
					return _Data.emplace_back(forward<Args>(args)...);
				}

				template<class Allocator>
				template<class ...Args>
				inline typename path_factory<Allocator>::iterator path_factory<Allocator>::emplace(const_iterator position, Args&& ...args) {
					return _Data.emplace(position, forward<Args>(args)...).
				}

				template<class Allocator>
				template<class InputIterator>
				inline typename path_factory<Allocator>::iterator path_factory<Allocator>::insert(const_iterator position, InputIterator first, InputIterator last) {
					return _Data.insert<InputIterator>(position, first, last);
				}

				template<class Allocator>
				inline path_factory<Allocator>::path_factory(const path_factory& pf)
					: _Data(pf._Data)
					, _Current_point(pf._Current_point)
					, _Last_move_to_point(pf._Last_move_to_point)
					, _Transform_matrix(pf._Transform_matrix)
					, _Origin(pf._Origin) {
				}

				template<class Allocator>
				inline path_factory<Allocator>::path_factory(path_factory&& pf) noexcept
					: _Data(move(pf._Data))
					, _Current_point(move(pf._Current_point))
					, _Last_move_to_point(move(pf._Last_move_to_point))
					, _Transform_matrix(move(pf._Transform_matrix))
					, _Origin(move(pf._Origin)) {
				}

				template<class Allocator>
				inline path_factory<Allocator>::path_factory(const path_factory& pf, const Allocator & a)
					: _Data(pf._Data, a)
					, _Current_point(pf._Current_point)
					, _Last_move_to_point(pf._Last_move_to_point)
					, _Transform_matrix(pf._Transform_matrix)
					, _Origin(pf._Origin) {
				}

				template<class Allocator>
				inline path_factory<Allocator>::path_factory(path_factory&& pf, const Allocator & a)
					: _Data(move(pf._Data), a)
					, _Current_point(move(pf._Current_point))
					, _Last_move_to_point(move(pf._Last_move_to_point))
					, _Transform_matrix(move(pf._Transform_matrix))
					, _Origin(move(pf._Origin)) {
				}

				template<class Allocator>
				inline path_factory<Allocator>::path_factory(initializer_list<value_type> il, const Allocator & a)
					: _Data(il, a)
					, _Current_point()
					, _Last_move_to_point()
					, _Transform_matrix()
					, _Origin() {
				}

				template <class Allocator>
				inline path_factory<Allocator>& path_factory<Allocator>::operator=(const path_factory& x) {
					_Data = x._Data;
					_Current_point = x._Current_point;
					_Last_move_to_point = x._Last_move_to_point;
					_Origin = x._Origin;
					return *this;
				}
				template<class Allocator>
				inline path_factory<Allocator>& path_factory<Allocator>::operator=(path_factory&& x) noexcept(allocator_traits<Allocator>::propagate_on_container_move_assignment::value || allocator_traits<Allocator>::is_always_equal::value) {
					::std::swap(_Data, x._Data);
					::std::swap(_Current_point, x._Current_point);
					::std::swap(_Last_move_to_point, x._Last_move_to_point);
					::std::swap(_Origin, x._Origin);
					return *this;
				}
				template<class Allocator>
				inline path_factory<Allocator>& path_factory<Allocator>::operator=(initializer_list<value_type> il) {
					_Data.clear();
					for (const auto& item : il) {
						_Data.push_back(item);
					}
					return *this;
				}
				template<class Allocator>
				inline void path_factory<Allocator>::assign(size_type n, const value_type& u) {
					_Data.assign(n, u);
				}
				template<class Allocator>
				inline void path_factory<Allocator>::assign(initializer_list<value_type> il) {
					_Data.assign(il);
				}
				template<class Allocator>
				inline typename path_factory<Allocator>::allocator_type path_factory<Allocator>::get_allocator() const noexcept {
					return _Data.allocator_type();
				}
				template<class Allocator>
				inline typename path_factory<Allocator>::iterator path_factory<Allocator>::begin() noexcept {
					return _Data.begin();
				}
				template<class Allocator>
				inline typename path_factory<Allocator>::const_iterator path_factory<Allocator>::begin() const noexcept {
					return _Data.begin();
				}
				template<class Allocator>
				inline typename path_factory<Allocator>::const_iterator path_factory<Allocator>::cbegin() const noexcept {
					return _Data.cbegin();
				}
				template<class Allocator>
				inline typename path_factory<Allocator>::iterator path_factory<Allocator>::end() noexcept {
					return _Data.end();
				}
				template<class Allocator>
				inline typename path_factory<Allocator>::const_iterator path_factory<Allocator>::end() const noexcept {
					return _Data.end();
				}
				template<class Allocator>
				inline typename path_factory<Allocator>::const_iterator path_factory<Allocator>::cend() const noexcept {
					return _Data.cend();
				}
				template<class Allocator>
				inline typename path_factory<Allocator>::reverse_iterator path_factory<Allocator>::rbegin() noexcept {
					return _Data.rbegin();
				}
				template<class Allocator>
				inline typename path_factory<Allocator>::const_reverse_iterator path_factory<Allocator>::rbegin() const noexcept {
					return _Data.rbegin();
				}
				template<class Allocator>
				inline typename path_factory<Allocator>::const_reverse_iterator path_factory<Allocator>::crbegin() const noexcept {
					return _Data.crbegin();
				}
				template<class Allocator>
				inline typename path_factory<Allocator>::reverse_iterator path_factory<Allocator>::rend() noexcept {
					return _Data.rend();
				}
				template<class Allocator>
				inline typename path_factory<Allocator>::const_reverse_iterator path_factory<Allocator>::rend() const noexcept {
					return _Data.rend();
				}
				template<class Allocator>
				inline typename path_factory<Allocator>::const_reverse_iterator path_factory<Allocator>::crend() const noexcept {
					return _Data.crend();
				}

				template<class Allocator>
				inline bool path_factory<Allocator>::empty() const noexcept {
					return _Data.empty();
				}

				template<class Allocator>
				inline typename path_factory<Allocator>::size_type path_factory<Allocator>::size() const noexcept {
					return _Data.size();
				}

				template<class Allocator>
				inline typename path_factory<Allocator>::size_type path_factory<Allocator>::max_size() const noexcept {
					return _Data.max_size();
				}

				template<class Allocator>
				inline typename path_factory<Allocator>::size_type path_factory<Allocator>::capacity() const noexcept {
					return _Data.capacity();
				}

				template<class Allocator>
				inline void path_factory<Allocator>::resize(size_type sz) {
					_Data.resize(sz);
				}

				template<class Allocator>
				inline void path_factory<Allocator>::resize(size_type sz, const value_type& c) {
					_Data.resize(sz, c);
				}

				template<class Allocator>
				inline void path_factory<Allocator>::reserve(size_type n) {
					_Data.reserve(n)
				}

				template<class Allocator>
				inline void path_factory<Allocator>::shrink_to_fit() {
					_Data.shrink_to_fit();
				}

				template<class Allocator>
				inline typename path_factory<Allocator>::reference path_factory<Allocator>::operator[](size_type n) {
					return _Data[n];
				}

				template<class Allocator>
				inline typename path_factory<Allocator>::const_reference path_factory<Allocator>::operator[](size_type n) const {
					return _Data[n];
				}

				template<class Allocator>
				inline typename path_factory<Allocator>::const_reference path_factory<Allocator>::at(size_type n) const {
					return _Data.at(n);
				}

				template<class Allocator>
				inline typename path_factory<Allocator>::reference path_factory<Allocator>::at(size_type n) {
					return _Data.at(n);
				}

				template<class Allocator>
				inline typename path_factory<Allocator>::reference path_factory<Allocator>::front() {
					return _Data.front();
				}

				template<class Allocator>
				inline typename path_factory<Allocator>::const_reference path_factory<Allocator>::front() const {
					return _Data.front();
				}

				template<class Allocator>
				inline typename path_factory<Allocator>::reference path_factory<Allocator>::back() {
					return _Data.back();
				}

				template<class Allocator>
				inline typename path_factory<Allocator>::const_reference path_factory<Allocator>::back() const {
					return _Data.back();
				}

				template<class Allocator>
				inline void path_factory<Allocator>::new_path() noexcept {
					_Data.emplace_back(in_place_type<path_data::new_path>);
				}

				template<class Allocator>
				inline void path_factory<Allocator>::close_path() noexcept {
					const vector_2d pt{};
					_Data.emplace_back(in_place_type<path_data::close_path>, pt);
				}

				template<class Allocator>
				inline void path_factory<Allocator>::arc_clockwise(const vector_2d& center, double radius, double angle1, double angle2) noexcept {
					_Data.emplace_back(in_place_type<path_data::arc_clockwise>, center, radius, angle1, angle2);
				}

				template<class Allocator>
				inline void path_factory<Allocator>::arc_clockwise(const circle& c, double angle1, double angle2) noexcept {
					_Data.emplace_back(in_place_type<path_data::arc_clockwise>, c, angle1, angle2);
				}

				template<class Allocator>
				inline void path_factory<Allocator>::arc_counterclockwise(const vector_2d& center, double radius, double angle1, double angle2) noexcept {
					_Data.emplace_back(in_place_type<path_data::arc_counterclockwise>, center, radius, angle1, angle2);
				}

				template<class Allocator>
				inline void path_factory<Allocator>::arc_counterclockwise(const circle& c, double angle1, double angle2) noexcept {
					_Data.emplace_back(in_place_type<path_data::arc_counterclockwise>, c, angle1, angle2);
				}

				template<class Allocator>
				inline void path_factory<Allocator>::ellipse(const vector_2d& ctr, double width, double height) noexcept {
					const auto m = _Transform_matrix;
					const auto o = _Origin;
					_Data.emplace_back(in_place_type<path_data::change_origin>, ctr);
					_Data.emplace_back(in_place_type<path_data::change_matrix>, matrix_2d::init_scale({ width/height, 1.0 }) * m);
					_Data.emplace_back(in_place_type<path_data::arc_clockwise>, ctr, height, 0.0, two_pi<double>);
					_Data.emplace_back(in_place_type<path_data::change_matrix>, m);
					_Data.emplace_back(in_place_type<path_data::change_origin>, o);
				}

				template<class Allocator>
				inline void path_factory<Allocator>::cubic_curve_to(const vector_2d& pt0, const vector_2d& pt1, const vector_2d& pt2) noexcept {
					_Data.emplace_back(in_place_type<path_data::abs_cubic_curve>, pt0, pt1, pt2);
				}

				template<class Allocator>
				inline void path_factory<Allocator>::line_to(const vector_2d& pt) noexcept {
					_Data.emplace_back(in_place_type<path_data::abs_line>, pt);
				}

				template<class Allocator>
				inline void path_factory<Allocator>::move_to(const vector_2d& pt) noexcept {
					_Data.emplace_back(in_place_type<path_data::abs_move>, pt);
				}

				template<class Allocator>
				inline void path_factory<Allocator>::quadratic_curve_to(const vector_2d& pt0, const vector_2d& pt1) noexcept {
					_Data.emplace_back(in_place_type<path_data::abs_quadratic_curve>, pt0, pt1);
				}

				template<class Allocator>
				inline void path_factory<Allocator>::rectangle(const experimental::io2d::rectangle& r) noexcept {
					_Data.emplace_back(in_place_type<path_data::abs_move>, vector_2d{ r.x(), r.y() });
					_Data.emplace_back(in_place_type<path_data::rel_line>, vector_2d{ r.width(), 0.0 });
					_Data.emplace_back(in_place_type<path_data::rel_line>, vector_2d{ 0.0, r.height() });
					_Data.emplace_back(in_place_type<path_data::rel_line>, vector_2d{ -r.width(), 0.0 });
					_Data.emplace_back(in_place_type<path_data::close_path>, vector_2d{ 0.0, 0.0 });
				}

				template<class Allocator>
				inline void path_factory<Allocator>::rel_cubic_curve_to(const vector_2d& dpt0, const vector_2d& dpt1, const vector_2d& dpt2) noexcept {
					_Data.emplace_back(in_place_type<path_data::rel_cubic_curve>, dpt0, dpt1, dpt2);
				}

				template<class Allocator>
				inline void path_factory<Allocator>::rel_line_to(const vector_2d& dpt) noexcept {
					_Data.emplace_back(in_place_type<path_data::rel_line>, dpt);
				}

				template<class Allocator>
				inline void path_factory<Allocator>::rel_move_to(const vector_2d& dpt) noexcept {
					_Data.emplace_back(in_place_type<path_data::rel_move>, dpt);
				}

				template<class Allocator>
				inline void path_factory<Allocator>::rel_quadratic_curve_to(const vector_2d& dpt0, const vector_2d& dpt1) noexcept {
					_Data.emplace_back(in_place_type<path_data::rel_quadratic_curve>, dpt0, dpt1);
				}

				template<class Allocator>
				inline void path_factory<Allocator>::transform_matrix(const matrix_2d& m) noexcept {
					_Transform_matrix = m;
					_Data.emplace_back(in_place_type<path_data::change_matrix>, m);
				}

				template<class Allocator>
				inline void path_factory<Allocator>::origin(const vector_2d& pt) noexcept {
					_Origin = pt;
					_Data.emplace_back(in_place_type<path_data::change_origin>, pt);
				}

				template<class Allocator>
				inline void path_factory<Allocator>::push_back(const value_type& x) {
					_Data.push_back(x);
				}

				template<class Allocator>
				inline void path_factory<Allocator>::push_back(value_type&& x) {
					_Data.push_back(move(x));
				}

				template<class Allocator>
				inline void path_factory<Allocator>::pop_back() {
					_Data.pop_back();
				}

				template<class Allocator>
				inline typename path_factory<Allocator>::iterator path_factory<Allocator>::insert(const_iterator position, const value_type& x) {
					return _Data.insert(position, x);
				}

				template<class Allocator>
				inline typename path_factory<Allocator>::iterator path_factory<Allocator>::insert(const_iterator position, value_type&& x) {
					return _Data.insert(position, x);
				}

				template<class Allocator>
				inline typename path_factory<Allocator>::iterator path_factory<Allocator>::insert(const_iterator position, size_type n, const value_type& x) {
					return _Data.insert(position, n, x);
				}

				template<class Allocator>
				inline typename path_factory<Allocator>::iterator path_factory<Allocator>::insert(const_iterator position, initializer_list<value_type> il) {
					return _Data.insert(position, il);
				}

				template<class Allocator>
				inline typename path_factory<Allocator>::iterator path_factory<Allocator>::erase(const_iterator position) {
					return _Data.erase(position);
				}

				template<class Allocator>
				inline typename path_factory<Allocator>::iterator path_factory<Allocator>::erase(const_iterator first, const_iterator last) {
					return _Data.erase(first, last);
				}

				template<class Allocator>
				inline void path_factory<Allocator>::swap(path_factory &pf) noexcept(allocator_traits<Allocator>::propagate_on_container_swap::value || allocator_traits<Allocator>::is_always_equal::value) {
					swap(_data, pf._Data);
					swap(_Current_point, pf._Current_point);
					swap(_Last_move_to_point, pf._Last_move_to_point);
					swap(_Origin, pf._Origin);
					swap(_Transform_matrix, pf._Transform_matrix);
				}

				template<class Allocator>
				inline void path_factory<Allocator>::clear() noexcept {
					_Data.clear();
					_Current_point.reset();
					_Transform_matrix = matrix_2d::init_identity();
					_Origin = {};
				}

				template<class Allocator>
				inline color_stop_group<Allocator>::color_stop_group(const Allocator& a) noexcept
					: _Stops(a) {
				}

				template<class Allocator>
				inline color_stop_group<Allocator>::color_stop_group(size_type n, const Allocator& a)
					: _Stops(n, a) {
				}

				template<class Allocator>
				inline color_stop_group<Allocator>::color_stop_group(size_type n, const value_type& value, const Allocator& a)
					: _Stops(n, value, a) {
				}

				template<class Allocator>
				inline color_stop_group<Allocator>::color_stop_group(const color_stop_group& x)
					: _Stops(x._Stops) {
				}

				template<class Allocator>
				inline color_stop_group<Allocator>::color_stop_group(color_stop_group&& x) noexcept
					: _Stops(::std::move(x._Stops)) {
				}

				template<class Allocator>
				inline color_stop_group<Allocator>::color_stop_group(const color_stop_group& x, const Allocator& a)
					: _Stops(x._Stops, a) {
				}

				template<class Allocator>
				inline color_stop_group<Allocator>::color_stop_group(color_stop_group&& x, const Allocator& a)
					: _Stops(move(x), a) {
				}

				template<class Allocator>
				inline color_stop_group<Allocator>::color_stop_group(initializer_list<value_type> il, const Allocator& a)
					: _Stops(il, a) {
				}

				template<class Allocator>
				inline color_stop_group<Allocator>::~color_stop_group() {
				}

				template<class Allocator>
				inline color_stop_group<Allocator>& color_stop_group<Allocator>::operator=(const color_stop_group& x) {
					_Stops = x._Stops;
					return *this;
				}

				template<class Allocator>
				inline color_stop_group<Allocator>& color_stop_group<Allocator>::operator=(color_stop_group&& x) noexcept(allocator_traits<Allocator>::propagate_on_container_move_assignment::value || allocator_traits<Allocator>::is_always_equal::value) {
					swap(_Stops, x._Stops);
					return *this;
				}

				template<class Allocator>
				inline color_stop_group<Allocator>& color_stop_group<Allocator>::operator=(initializer_list<value_type> il) {
					_Stops = il;
					return *this;
				}

				template<class Allocator>
				inline void color_stop_group<Allocator>::assign(size_type n, const value_type& u) {
					_Stops.assign(n, u);
				}

				template<class Allocator>
				inline void color_stop_group<Allocator>::assign(initializer_list<value_type> il) {
					_Stops.assign(il);
				}

				template<class Allocator>
				inline typename color_stop_group<Allocator>::allocator_type color_stop_group<Allocator>::get_allocator() const noexcept {
					return allocator_type();
				}

				template<class Allocator>
				inline typename color_stop_group<Allocator>::iterator color_stop_group<Allocator>::begin() noexcept {
					return _Stops.begin();
				}

				template<class Allocator>
				inline typename color_stop_group<Allocator>::const_iterator color_stop_group<Allocator>::begin() const noexcept {
					return _Stops.begin();
				}

				template<class Allocator>
				inline typename color_stop_group<Allocator>::const_iterator color_stop_group<Allocator>::cbegin() const noexcept {
					return _Stops.cbegin();
				}

				template<class Allocator>
				inline typename color_stop_group<Allocator>::iterator color_stop_group<Allocator>::end() noexcept {
					return _Stops.end();
				}

				template<class Allocator>
				inline typename color_stop_group<Allocator>::const_iterator color_stop_group<Allocator>::end() const noexcept {
					return _Stops.end();
				}

				template<class Allocator>
				inline typename color_stop_group<Allocator>::const_iterator color_stop_group<Allocator>::cend() const noexcept {
					return _Stops.cend();
				}

				template<class Allocator>
				inline typename color_stop_group<Allocator>::reverse_iterator color_stop_group<Allocator>::rbegin() noexcept {
					return _Stops.rbegin();
				}

				template<class Allocator>
				inline typename color_stop_group<Allocator>::const_reverse_iterator color_stop_group<Allocator>::rbegin() const noexcept {
					return _Stops.rbegin();
				}

				template<class Allocator>
				inline typename color_stop_group<Allocator>::const_reverse_iterator color_stop_group<Allocator>::crbegin() const noexcept {
					return _Stops.crbegin();
				}

				template<class Allocator>
				inline typename color_stop_group<Allocator>::reverse_iterator color_stop_group<Allocator>::rend() noexcept {
					return _Stops.rend();
				}

				template<class Allocator>
				inline typename color_stop_group<Allocator>::const_reverse_iterator color_stop_group<Allocator>::rend() const noexcept {
					return _Stops.rend();
				}

				template<class Allocator>
				inline typename color_stop_group<Allocator>::const_reverse_iterator color_stop_group<Allocator>::crend() const noexcept {
					return _Stops.crend();
				}

				template<class Allocator>
				inline bool color_stop_group<Allocator>::empty() const noexcept {
					return _Stops.empty();
				}

				template<class Allocator>
				inline typename color_stop_group<Allocator>::size_type color_stop_group<Allocator>::size() const noexcept {
					return _Stops.size();
				}

				template<class Allocator>
				inline typename color_stop_group<Allocator>::size_type color_stop_group<Allocator>::max_size() const noexcept {
					return _Stops.max_size();
				}

				template<class Allocator>
				inline typename color_stop_group<Allocator>::size_type color_stop_group<Allocator>::capacity() const noexcept {
					return _Stops.capacity();
				}

				template<class Allocator>
				inline void color_stop_group<Allocator>::resize(size_type sz) {
					_Stops.resize(sz);
				}

				template<class Allocator>
				inline void color_stop_group<Allocator>::resize(size_type sz, const value_type& c) {
					_Stops.resize(sz, c);
				}

				template<class Allocator>
				inline void color_stop_group<Allocator>::reserve(size_type n) {
					_Stops.reserve(n);
				}

				template<class Allocator>
				inline void color_stop_group<Allocator>::shrink_to_fit() {
					_Stops.shrink_to_fit();
				}

				template<class Allocator>
				inline typename color_stop_group<Allocator>::reference color_stop_group<Allocator>::operator[](size_type n) {
					return _Stops[n];
				}

				template<class Allocator>
				inline typename color_stop_group<Allocator>::const_reference color_stop_group<Allocator>::operator[](size_type n) const {
					return _Stops[n];
				}

				template<class Allocator>
				inline typename color_stop_group<Allocator>::const_reference color_stop_group<Allocator>::at(size_type n) const {
					return _Stops.at(n);
				}

				template<class Allocator>
				inline typename color_stop_group<Allocator>::reference color_stop_group<Allocator>::at(size_type n) {
					return _Stops.at(n);
				}

				template<class Allocator>
				inline typename color_stop_group<Allocator>::reference color_stop_group<Allocator>::front() {
					return _Stops.front();
				}

				template<class Allocator>
				inline typename color_stop_group<Allocator>::const_reference color_stop_group<Allocator>::front() const {
					return _Stops.front();
				}

				template<class Allocator>
				inline typename color_stop_group<Allocator>::reference color_stop_group<Allocator>::back() {
					return _Stops.back();
				}

				template<class Allocator>
				inline typename color_stop_group<Allocator>::const_reference color_stop_group<Allocator>::back() const {
					return _Stops.back();
				}

				template<class Allocator>
				inline void color_stop_group<Allocator>::push_back(const value_type& x) {
					_Stops.push_bacl(x);
				}

				template<class Allocator>
				inline void color_stop_group<Allocator>::push_back(value_type&& x) {
					_Stops.push_back(move(x));
				}

				template<class Allocator>
				inline void color_stop_group<Allocator>::pop_back() {
					_Stops.pop_back();
				}

				template<class Allocator>
				inline typename color_stop_group<Allocator>::iterator color_stop_group<Allocator>::insert(const_iterator position, const value_type & x) {
					return _Stops.insert(position, x);
				}

				template<class Allocator>
				inline typename color_stop_group<Allocator>::iterator color_stop_group<Allocator>::insert(const_iterator position, value_type && x) {
					return _Stops.insert(position, ::std::move(x));
				}

				template<class Allocator>
				inline typename color_stop_group<Allocator>::iterator color_stop_group<Allocator>::insert(const_iterator position, size_type n, const value_type& x) {
					return _Stops.insert(position, n, x);
				}

				template<class Allocator>
				inline typename color_stop_group<Allocator>::iterator color_stop_group<Allocator>::insert(const_iterator position, initializer_list<value_type> il) {
					return _Stops.insert(position, il);
				}

				template<class Allocator>
				inline typename color_stop_group<Allocator>::iterator color_stop_group<Allocator>::erase(const_iterator position) {
					return _Stops.erase(position);
				}

				template<class Allocator>
				inline typename color_stop_group<Allocator>::iterator color_stop_group<Allocator>::erase(const_iterator first, const_iterator last) {
					return _Stops.erase(first, last);
				}

				template<class Allocator>
				inline void color_stop_group<Allocator>::swap(color_stop_group& other) noexcept(allocator_traits<Allocator>::propagate_on_container_swap::value || allocator_traits<Allocator>::is_always_equal::value) {
					_Stops.swap(other._Stops);
				}

				template<class Allocator>
				inline void color_stop_group<Allocator>::clear() noexcept {
					_Stops.clear();
				}

				template<class Allocator>
				template<class InputIterator>
				inline color_stop_group<Allocator>::color_stop_group(InputIterator first, InputIterator last, const Allocator& a)
					: _Stops(first, last, a) {
				}

				template<class Allocator>
				template<class InputIterator>
				inline void color_stop_group<Allocator>::assign(InputIterator first, InputIterator last) {
					_Stops.assign<InputIterator>(first, last);
				}

				template<class Allocator>
				template<class ...Args>
				inline typename color_stop_group<Allocator>::reference color_stop_group<Allocator>::emplace_back(Args&& ...args) {
					_Stops.emplace_back<Args&&...>(::std::forward<Args>(args)...);
					return _Stops.back();
				}

				template<class Allocator>
				template<class ...Args>
				inline typename color_stop_group<Allocator>::iterator color_stop_group<Allocator>::emplace(const_iterator position, Args && ...args) {
					return _Stops.emplace(position, ::std::forward<Args>(args)...);
				}

				template<class Allocator>
				template<class InputIterator>
				inline typename color_stop_group<Allocator>::iterator color_stop_group<Allocator>::insert(const_iterator position, InputIterator first, InputIterator last) {
					return _Stops.insert(position, first, last);
				}


				template <class Allocator>
				inline brush::brush(const vector_2d& begin, const vector_2d& end,
					const color_stop_group<Allocator>& csg)
					: _Brush()
					, _Image_surface()
					, _Brush_type(brush_type::linear)
					, _Extend(::std::experimental::io2d::tiling::none)
					, _Filter(::std::experimental::io2d::filter::good)
					, _Matrix(matrix_2d::init_identity()) {
					_Brush = shared_ptr<cairo_pattern_t>(cairo_pattern_create_linear(begin.x(), begin.y(), end.x(), end.y()), &cairo_pattern_destroy);
					_Throw_if_failed_cairo_status_t(cairo_pattern_status(_Brush.get()));

					for (const color_stop& stop : csg) {
						cairo_pattern_add_color_stop_rgba(_Brush.get(), stop.offset(), stop.color().r(), stop.color().g(), stop.color().b(), stop.color().a());
					}
					_Throw_if_failed_cairo_status_t(cairo_pattern_status(_Brush.get()));
				}

				template <class Allocator>
				inline brush::brush(const vector_2d& begin, const vector_2d& end,
					const color_stop_group<Allocator>& csg, error_code& ec) noexcept
					: _Brush()
					, _Image_surface()
					, _Brush_type(brush_type::linear)
					, _Extend(::std::experimental::io2d::tiling::none)
					, _Filter(::std::experimental::io2d::filter::good)
					, _Matrix(matrix_2d::init_identity()) {
					try {
						_Brush = shared_ptr<cairo_pattern_t>(cairo_pattern_create_linear(begin.x(), begin.y(), end.x(), end.y()), &cairo_pattern_destroy);
					}
					catch (const bad_alloc&) {
						ec = make_error_code(errc::not_enough_memory);
						_Brush.reset();
						return;
					}
					catch (const ::std::length_error&) {
						_Brush.reset();
						ec = ::std::make_error_code(::std::errc::not_enough_memory);
						return;
					}

					auto status = cairo_pattern_status(_Brush.get());
					if (status != CAIRO_STATUS_SUCCESS) {
						ec = _Cairo_status_t_to_std_error_code(status);
						_Brush.reset();
						return;
					}

					for (const color_stop& stop : csg) {
						cairo_pattern_add_color_stop_rgba(_Brush.get(), stop.offset(), stop.color().r(), stop.color().g(), stop.color().b(), stop.color().a());
					}
					status = cairo_pattern_status(_Brush.get());
					if (status != CAIRO_STATUS_SUCCESS) {
						ec = _Cairo_status_t_to_std_error_code(status);
						_Brush.reset();
						return;
					}
					ec.clear();
				}

				template <class Allocator>
				inline brush::brush(const circle& start, const circle& end,
					const color_stop_group<Allocator>& csg)
					: _Brush()
					, _Brush_type(brush_type::radial)
					, _Extend(::std::experimental::io2d::tiling::none)
					, _Filter(::std::experimental::io2d::filter::good)
					, _Matrix(matrix_2d::init_identity()) {
					_Brush = shared_ptr<cairo_pattern_t>(cairo_pattern_create_radial(start.center().x(), start.center().y(), start.radius(), end.center().x(), end.center().y(), end.radius()), &cairo_pattern_destroy);
					_Throw_if_failed_cairo_status_t(cairo_pattern_status(_Brush.get()));

					for (const color_stop& stop : csg) {
						cairo_pattern_add_color_stop_rgba(_Brush.get(), stop.offset(), stop.color().r(), stop.color().g(), stop.color().b(), stop.color().a());
					}
					_Throw_if_failed_cairo_status_t(cairo_pattern_status(_Brush.get()));
				}

				template <class Allocator>
				inline brush::brush(const circle& start, const circle& end,
					const color_stop_group<Allocator>& csg, error_code& ec) noexcept
					: _Brush()
					, _Image_surface()
					, _Brush_type(brush_type::radial)
					, _Extend(::std::experimental::io2d::tiling::none)
					, _Filter(::std::experimental::io2d::filter::good)
					, _Matrix(matrix_2d::init_identity()) {
					try {
						_Brush = shared_ptr<cairo_pattern_t>(cairo_pattern_create_radial(start.center().x(), start.center().y(), start.radius(), end.center().x(), end.center().y(), end.radius()), &cairo_pattern_destroy);
					}
					catch (const bad_alloc&) {
						ec = make_error_code(errc::not_enough_memory);
						_Brush.reset();
						return;
					}
					catch (const ::std::length_error&) {
						_Brush.reset();
						ec = ::std::make_error_code(::std::errc::not_enough_memory);
						return;
					}

					auto status = cairo_pattern_status(_Brush.get());
					if (status != CAIRO_STATUS_SUCCESS) {
						ec = _Cairo_status_t_to_std_error_code(status);
						_Brush.reset();
						return;
					}

					for (const color_stop& stop : csg) {
						cairo_pattern_add_color_stop_rgba(_Brush.get(), stop.offset(), stop.color().r(), stop.color().g(), stop.color().b(), stop.color().a());
					}
					status = cairo_pattern_status(_Brush.get());
					if (status != CAIRO_STATUS_SUCCESS) {
						ec = _Cairo_status_t_to_std_error_code(status);
						_Brush.reset();
						return;
					}
					ec.clear();
				}

				template <class... _Args>
				void _Add_color_stop(cairo_pattern_t*, _Args&&...);

#if _Inline_namespace_conditional_support_test
			}
#endif
		}
	}
}

#endif

