#pragma once
#include "libchemist/hamiltonian/operator.hpp"
#include "libchemist/hamiltonian/type_traits.hpp"
#include <memory>

namespace libchemist {
namespace detail_ {
class DerivedOperatorPIMPL;
} // namespace detail_


/** @brief A class to store an manipulate operators which are 
 *         collections of other operators
 *
 *  This class provides a set of public APIs for the storage and
 *  manipulations of collections of operators.
 *
 *  Example usage:
 *      DerivedOperator d_op( ElectronKinetic{}, ElectronElectronCoulomb{} );
 *      d_op.add_term( NuclearKinetic{} );
 */
class DerivedOperator {

public:

    /** @brief Creates a new DerivedOperator instance containing no operators.
     *
     *  This ctor can be used to create a new DerivedOperator instance which
     *  contains no operators. Operators may be added by calling `add_term`
     *  or `add_terms`.
     *
     *  @throw None No throw gurantee.
     *
     *  Complexity: Constant
     */
    DerivedOperator() noexcept;

    /** @brief Creates a new DerivedOperator instance by deep copying another
     *         instance.
     *
     *  This ctor can be used to create a new DerivedOperator instance which
     *  contains a deep copy of @p other
     *
     *  @param[in] other DerivedOperator instance to copy
     *
     *  @throw std::bad_alloc if there is insufficient memory to copy the
     *                        DerivedOperator instance. Strong exception gurantee.
     *
     *  Complexity: Linear in the number of operators which comprise @p other.
     */
    DerivedOperator( const DerivedOperator& other );

    /** @brief Creates a new DerivedOperator instance by taking ownership of
     *         another instance's state.
     *
     *  This ctor will create a new DerivedOperator instance by taking ownership of
     *  @p other's state.
     *
     *  @param[in,out] other The instance whose state will be transferred to the
     *                       resulting instance. After the operation @p other will
     *                       not contain a PIMPL and will thus not be usable until
     *                       another PIMPL-containing DerivedOperator instance is
     *                       assigned to it.
     *
     *  @throw None No throw guarantee.
     *
     *  Complexity: Constant.
     */
    DerivedOperator( DerivedOperator&& other ) noexcept;

    /// Defaulted no-throw dtor
    virtual ~DerivedOperator() noexcept;

    
    /** @brief Deep copy the state of another DerivedOperator instance to the
     *         current instance.
     *
     *  This function will deep copy the state of @p other to the current
     *  instance. The state previously held by this instance will be released.
     *
     *  @param[in] other The DerivedOperator instance to deep copy.
     *
     *  @return A reference to the current instance which contains a deep copy 
     *          of @p other.
     *
     *  @throw std::bad_alloc if there is insufficient memory to copy the
     *                        DerivedOperator instance @p other. Strong exception 
     *                        gurantee.
     *
     *  Complexity: Linear in the number of operators which comprise @p other.
     */
    DerivedOperator& operator=( const DerivedOperator& other );

    /** @brief Transfer the ownership of the state of another DerivedOperator
     *         instance to the current instance.
     *
     *  This function will transfer ownership of @p other's state to the
     *  current instance. The state previously held by this instance will 
     *  be released.
     *
     *  @param[in,out] other The DerivedOperator instance whose state is to be 
     *                       taken. @p other will no longer contain a valid
     *                       PIMPL and will need to be reassigned prior to
     *                       future usage.
     *
     *  @return The current instance after taking ownership of @p other's state.
     *
     *  @throw None No throw guarantee.
     *
     *  Complexity: Constant.
     */
    DerivedOperator& operator=( DerivedOperator&& other ) noexcept;

    /** @brief Creates a new DerivedOperator instance from a collection of 
     *  Operator instances.
     *
     *  This ctor can be used to create a DerivedOperator instance from
     *  a collection of Operator instances. Additional terms may be
     *  added by invoking `add_term` or `add_terms`.
     *
     *  @tparam Args Parameter pack of passed Operator types, must all
     *               be derived from Operator.
     *  
     *  @param[in] args Operator instances from which to construct the 
     *                  DerivedOperator.
     *
     *  @throw std::bad_alloc if there is insufficient memory either to create the
     *                        PIMPL or to store the internal state of any Operator
     *                        instance. Basic exception gurantee.
     *
     *  Complexity: Linear in the size of @p args
     */
    template <typename... Args,
      typename = std::enable_if_t< detail_::all_are_operator_v<Args...> > 
    >
    DerivedOperator( Args&&... args ) : DerivedOperator() {
        add_terms( std::forward<Args>(args)... );
    }



    /** @brief Add an additonal operator to the DerivedOperator.
     *
     *  This function adds an operator to this DerivedOperator by copy. If the 
     *  DerivedOperator is in a PIMPL-less state, this function will 
     *  generate a PIMPL instance. 
     *
     *  @tparam OpType The strong type of the operator to add to the 
     *                 DerivedOperator. Must be derived from Operator.
     *
     *  @param[in] op The operator to add to this DerivedOperator by copy.
     *
     *  @return A reference to the current DerivedOperator instance.
     *
     *  @throw std::bad_alloc if there is insufficient memory either to create the
     *                        PIMPL or to store the internal state of the Operator
     *                        instance. Basic exception gurantee.
     *
     *  Complexity: Constant
     */
    template <typename OpType>
    detail_::enable_if_operator_t< OpType, DerivedOperator& > 
      add_term( const OpType& op ) {
        add_term_( typeid(OpType), std::make_shared<OpType>(op) );
        return *this;
    }

    /** @brief Add an additonal operator to the DerivedOperator.
     *
     *  This function adds an operator to this DerivedOperator by move. If the 
     *  DerivedOperator is in a PIMPL-less state, this function will 
     *  generate a PIMPL instance. 
     *
     *  @tparam OpType The strong type of the operator to add to the 
     *                 DerivedOperator. Must be derived from Operator.
     *
     *  @param[in] op The operator to add to this DerivedOperator by move.
     *
     *  @return A reference to the current DerivedOperator instance.
     *
     *  @throw std::bad_alloc if there is insufficient memory either to create the
     *                        PIMPL or to store the internal state of the Operator
     *                        instance. Basic exception gurantee.
     *
     *  Complexity: Constant
     */
    template <typename OpType>
    detail_::enable_if_operator_t< OpType, DerivedOperator& > 
      add_term( OpType&& op ) {
        add_term_( typeid(OpType), std::make_shared<OpType>(std::move(op)) );
        return *this;
    }

    /** @brief Add several additonal operators to the DerivedOperator.
     *
     *  This function adds several operators to this DerivedOperator. If the 
     *  DerivedOperator is in a PIMPL-less state, this function will 
     *  generate a PIMPL instance. 
     *
     *  @tparam Ops The strong types of the operators to add to the 
     *              DerivedOperator. All must be derived from Operator.
     *
     *  @param[in] ops The operators to add to this DerivedOperator.
     *
     *  @return A reference to the current DerivedOperator instance.
     *
     *  @throw std::bad_alloc if there is insufficient memory either to create the
     *                        PIMPL or to store the internal state of any Operator
     *                        instance. Basic exception gurantee.
     *
     *  Complexity: Linear in the size of @p ops.
     */
    template <typename... Ops>
    std::enable_if_t< detail_::all_are_operator_v<Ops...>, DerivedOperator& >
      add_terms( Ops&&... ops ) {
        (add_term( std::forward<Ops>(ops) ),...);
        return *this;
    }

    template <typename OpType>
    using get_return_type = typename std::vector<std::shared_ptr<OpType>>;


    /** @brief Returns the collection of operator terms matching a specific 
     *         operator type contained in this DerivedOperator.
     *
     *  Obtains the unique collection of operator terms contained in the
     *  current DerivedOperator instance which match a specific Operator strong
     *  type. If the Operator is not represented in this DerivedOperator or if
     *  the DerivedOperator is in a PIMPL-less state, an empty collection is 
     *  returned.
     *
     *  @tparam OpType The Operator type from which to query the DerivedOperator
     *
     *  @returns A vector of shared_ptr<OpType> which contains all of the
     *           operators in this DerivedOperator that match OpType.
     *
     *  @throw std::bad_alloc if there is insufficient memroy to allocate
     *                        the return value
     *
     *  Complexity: Linear in the number of operators contained in this
     *              DerivedOperator.
     */
    template <typename OpType>
    detail_::enable_if_operator_t< OpType, get_return_type<OpType> > 
      get_terms() const {
        auto type_erased_terms = get_terms_( typeid(OpType) );
        get_return_type<OpType> ret_terms( type_erased_terms.size() );
        std::transform( type_erased_terms.begin(), type_erased_terms.end(),
          ret_terms.begin(), std::dynamic_pointer_cast<OpType,Operator> );
        return ret_terms;
    }

    /** @brief Return whether an Operator type is represented in this 
     *         DerivedOperator.
     *  
     *  Return a boolean which indicates whether or not a particlar Operator type
     *  is represented in this DerivedOperator. If this DerivedOperator is in 
     *  a PIMPL-less state, `false` is returned.
     *
     *  @tparam OpType The Operator type from which to query the DerivedOperator
     *
     *  @returns `true` if `OpType` is represented in this DerivedOperator, `false`
     *           otherwise.
     *
     *  @throw None No throw gurantee
     *
     *  Complexity: Linear in the number of operators contained in this
     *              DerivedOperator.
     */
    template <typename OpType>
    detail_::enable_if_operator_t< OpType, bool > has_term() const noexcept {
        return has_term_( typeid(OpType) );
    }


    bool operator==( const DerivedOperator& other ) const;
    bool operator!=( const DerivedOperator& other ) const;

    template <typename DerivedOpType>
    inline bool is_equal( const DerivedOpType& other ) const noexcept {
      return is_equal_impl(other) and other.is_equal_impl(*this);
    }

protected:
    virtual bool is_equal_impl( const DerivedOperator& other ) const noexcept = 0;

private:

    /// Type erased private API for `add_term` which delegates to 
    /// HamiltonanPIMPL::add_term
    void add_term_( std::type_index index, std::shared_ptr<Operator>&& );

    /// Type erased private API for `get_terms` which delegates to 
    /// HamiltonanPIMPL::get_terms
    get_return_type<Operator> get_terms_( std::type_index index ) const;

    /// Type erased private API for `has_term` which delegates to 
    /// HamiltonanPIMPL::has_term
    bool has_term_( std::type_index index ) const noexcept;


    /// The instance actually implementing the API
    std::unique_ptr<detail_::DerivedOperatorPIMPL> pimpl_;


}; // class DerivedOperator


} // namespace libchemist
