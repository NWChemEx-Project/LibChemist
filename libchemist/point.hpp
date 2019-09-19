#pragma once
#include <memory>

namespace libchemist {
namespace detail_ {
template<typename T>
class PointPIMPL;
}

/** @brief An object that is associated with a point in 3-D Cartesian space.
 *
 *  This class is largely intended to be used as a mix-in for code factorization
 *  purposes. More specifically many of the point-centered quantities in
 *  LibChemist such as Gaussians, Shells, and Atoms, have a Cartesian coordinate
 *  associated with them. This class factors the machinery common to those
 *  classes out.
 *
 *  The Point class always owns the memory associated with it. Its counterpart,
 *  PointView, on the other hand always aliases the memory associated with it.
 *
 *  @note The implementation of this class is contained in point.cpp. Thus to
 *        use this class with a new type one needs to add an explicit template
 *        instantiation for that type to point.cpp (and declare it in this
 *        file).
 *
 * @tparam T The type used to store each Cartesian coordinate. Assumed to be a
 *         POD type like double or float.
 */
template<typename T>
class Point {
public:
    /// Type of a read/write reference to a component of the point
    using reference = T&;
    /// Type of a read-only reference to a component of the point
    using const_reference = const T&;
    /// Type used for indexing and offsets
    using size_type = std::size_t;

    /** @brief Creates a new Point instance situated at the origin.
     *
     *  This ctor will create a new Point instance. The instance will sit at the
     *  origin and can be translated by directly modifying the references
     *  returned through `coord` or any of the convience `x`, `y`, `z`
     *  functions.
     *
     *  @throw std::bad_alloc if there is insufficient memory to allocate the
     *         PIMPL. Strong throw guarantee.
     */
    Point();

    /** @brief Initializes a new Point instance so that it resides at the
     *         specified coordinate.
     *
     *  @param[in] x The value of the first component of the Point's location.
     *  @param[in] y The value of the second component of the Point's location.
     *  @param[in] z The value of the third component of the Point's location.
     *
     *  @throw std::bad_alloc if there is insufficient memory to allocate the
     *         PIMPL. Strong throw guarantee.
     */
    Point(T x, T y, T z);

    /** @brief Standard defaulted dtor
     *
     *  This dtor will free up the memory associated with the PIMPL. As a
     *  result, all references to the Point's coordinates will be invalidated.
     *
     *  @throw none No throw guarantee.
     */
    virtual ~Point() noexcept;

    /** @brief Returns the @p q-th coordinate of the point.
     *
     *  This function can be used to retrieve one of the point's coordinates
     *  by index. The resulting coordinate is read-/write-able.
     *
     *  @param[in] q The index of the coordinate to retrieve. Should be in the
     *             range [0, 3).
     *  @return The requested coordinate in a read-/write-able form.
     *
     *  @throw std::out_of_range if @p q is not in the range [0, 3). Strong
     *         throw guarantee.
     */
    reference coord(size_type q);

    /** @brief Returns the @p q-th coordinate of the point in a read-only format
     *
     *  This function can be used to retrieve one of the point's coordinates
     *  by index. The resulting coordinate is read-only.
     *
     *  @param[in] q The index of the coordinate to retrieve. Should be in the
     *             range [0, 3).
     *  @return The requested coordinate as a read-only reference.
     *
     *  @throw std::out_of_range if @p q is not in the range [0, 3). Strong
     *         throw guarantee.
     */
    const_reference coord(size_type q) const;

    /** @brief Returns the x-coordinate of the point in a read/write format.
     *
     *  This function is a convenience function for calling `coord(0)`.
     *
     *  @return A read/write reference to the x-coordinate of the point.
     *
     *  @throw none No throw guarantee.
     */
    reference x() noexcept { return coord(0); }

    /** @brief Returns the x-coordinate of the point in a read-only format.
     *
     *  This function is a convenience function for calling `coord(0) const`.
     *
     *  @return A read-only reference to the x-coordinate of the point.
     *
     *  @throw none No throw guarantee.
     */
    const_reference x() const noexcept { return coord(0); }

    /** @brief Returns the y-coordinate of the point in a read/write format.
     *
     *  This function is a convenience function for calling `coord(1)`.
     *
     *  @return A read/write reference to the y-coordinate of the point.
     *
     *  @throw none No throw guarantee.
     */
    reference y() noexcept { return coord(1); }

    /** @brief Returns the y-coordinate of the point in a read-only format.
     *
     *  This function is a convenience function for calling `coord(1) const`.
     *
     *  @return A read-only reference to the y-coordinate of the point.
     *
     *  @throw none No throw guarantee.
     */
    const_reference y() const noexcept { return coord(1); }

    /** @brief Returns the z-coordinate of the point in a read/write format.
     *
     *  This function is a convenience function for calling `coord(2)`.
     *
     *  @return A read/write reference to the z-coordinate of the point.
     *
     *  @throw none No throw guarantee.
     */
    reference z() noexcept { return coord(2); }

    /** @brief Returns the z-coordinate of the point in a read-only format.
     *
     *  This function is a convenience function for calling `coord(2) const`.
     *
     *  @return A read-only reference to the z-coordinate of the point.
     *
     *  @throw none No throw guarantee.
     */
    const_reference z() const noexcept { return coord(2); }

private:
    /// The instance actually implementing the Point class
    std::unique_ptr<detail_::PointPIMPL<T>> m_pimpl_;
};

/** @brief Compares two Points for equality.
 *
 *  @relates Point
 *
 *  Two Point instances are equal if each of their three components compare
 *  bitwise equivalent (*i.e.*, Points with x-coordinates 3.12345 and 3.12346
 *  respectively will not compare equal).
 *
 *  @tparam T The type used to store the Points' components
 *
 *  @param[in] lhs The Point on the left-side of the operator.
 *  @param[in] rhs The Point on the right-side of the operator.
 *
 *  @return True if the Points are equal and false otherwise.
 *
 *  @throw none No throw guarantee.
 */
template<typename T>
bool operator==(const Point<T>& lhs, const Point<T>& rhs) noexcept {
    return std::tie(lhs.x(), lhs.y(), lhs.z()) ==
           std::tie(rhs.x(), rhs.y(), rhs.z());
}

/** @brief Determines if two Points are different.
 *
 *  @relates Point
 *
 *  Two Point instances are equal if each of their three components compare
 *  bitwise equivalent (*i.e.*, Points with x-coordinates 3.12345 and 3.12346
 *  respectively will not compare equal). We define "different" as not equal.
 *
 *  @tparam T The type used to store the Points' components
 *
 *  @param[in] lhs The Point on the left-side of the operator.
 *  @param[in] rhs The Point on the right-side of the operator.
 *
 *  @return False if the Points are equal and true otherwise.
 *
 *  @throw none No throw guarantee.
 */
template<typename T>
bool operator!=(const Point<T>& lhs, const Point<T>& rhs) noexcept {
    return !(lhs == rhs);
}

extern template class Point<double>;
extern template class Point<float>;

} // namespace libchemist
