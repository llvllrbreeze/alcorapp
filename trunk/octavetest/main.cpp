
#define LTEST6

#ifdef LTEST0

// solving A * X = B
// using driver function gesv()


#include <cstddef>
#include <iostream>
#include <complex>
#include <boost/numeric/bindings/lapack/gesv.hpp>
#include <boost/numeric/bindings/traits/ublas_matrix.hpp>
#include <boost/numeric/bindings/traits/std_vector.hpp>
#include "utils.h"

namespace ublas = boost::numeric::ublas;
namespace lapack = boost::numeric::bindings::lapack;

using std::size_t; 
using std::cout;
using std::endl; 

typedef std::complex<double> cmpx_t; 

typedef ublas::matrix<double, ublas::column_major> m_t;
typedef ublas::matrix<cmpx_t, ublas::column_major> cm_t;

int main() {

  cout << endl; 
  cout << "real system:" << endl << endl; 

  size_t n = 5;   
  m_t a (n, n);   // system matrix 

  size_t nrhs = 2; 
  m_t x (n, nrhs), b (n, nrhs);  // b -- right-hand side matrix

  std::vector<int> ipiv (n);  // pivot vector

  init_symm (a); 
  //     [n   n-1 n-2  ... 1]
  //     [n-1 n   n-1  ... 2]
  // a = [n-2 n-1 n    ... 3]
  //     [        ...       ]
  //     [1   2   ...  n-1 n]

  m_t aa (a);  // copy of a, because a is `lost' after gesv()

  for (int i = 0; i < x.size1(); ++i) {
    x (i, 0) = 1.;
    x (i, 1) = 2.; 
  }
  b = prod (a, x); 

  print_m (a, "A"); 
  cout << endl; 
  print_m (b, "B"); 
  cout << endl; 

  lapack::gesv (a, ipiv, b);   // solving the system, b contains x 
  print_m (b, "X"); 
  cout << endl; 

  x = prod (aa, b); 
  print_m (x, "B = A X"); 
  cout << endl; 

  ////////////////////////////////////////////////////////

  cout << endl; 
  cout << "complex system:" << endl << endl; 
  cm_t ca (3, 3), cb (3, 1), cx (3, 1);
  std::vector<int> ipiv2 (3); 

  ca (0, 0) = cmpx_t (3, 0);
  ca (0, 1) = cmpx_t (4, 2);
  ca (0, 2) = cmpx_t (-7, 5);
  ca (1, 0) = cmpx_t (4, -2);
  ca (1, 1) = cmpx_t (-5, 0);
  ca (1, 2) = cmpx_t (0, -3);
  ca (2, 0) = cmpx_t (-7, -5);
  ca (2, 1) = cmpx_t (0, 3);
  ca (2, 2) = cmpx_t (2, 0);
  print_m (ca, "CA"); 
  cout << endl; 

  for (int i = 0; i < cx.size1(); ++i) 
    cx (i, 0) = cmpx_t (1, -1); 
  cb = prod (ca, cx); 
  print_m (cb, "CB"); 
  cout << endl; 
  
  int ierr = lapack::gesv (ca, ipiv2, cb); 
  if (ierr == 0) 
    print_m (cb, "CX");
  else
    cout << "matrix is singular" << endl; 

  cout << endl; 

  getchar();

}


#endif


#ifdef LTEST1

  Permission to copy, use, modify, sell and
  distribute this software is granted provided this copyright notice appears
  in all copies. This software is provided "as is" without express or implied
  warranty, and with no claim as to its suitability for any purpose.
  Copyright Toon Knapen, Karl Meerbergen


#include "random.hpp"

#include <boost/numeric/bindings/lapack/geqrf.hpp>
#include <boost/numeric/bindings/lapack/ormqr.hpp>
#include <boost/numeric/bindings/traits/ublas_matrix.hpp>
#include <boost/numeric/bindings/traits/ublas_vector.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <iostream>
#include <limits>


namespace ublas = boost::numeric::ublas;
namespace lapack = boost::numeric::bindings::lapack;


// Randomize a matrix
template <typename M>
void randomize(M& m) {
   typedef typename M::size_type  size_type ;
   typedef typename M::value_type value_type ;

   size_type size1 = m.size1() ;
   size_type size2 = m.size2() ;

   for (size_type i=0; i<size2; ++i) {
      for (size_type j=0; j<size1; ++j) {
         m(j,i) = random_value< value_type >() ;
      }
   }
} // randomize()


template <typename T>
struct transpose {
   static const char value ;
};

template <typename T>
const char transpose<T>::value = 'T';



template <typename T>
struct transpose< std::complex<T> > {
   static const char value ;
};

template <typename T>
const char transpose< std::complex<T> >::value = 'C';

template <typename M>
ublas::triangular_adaptor<const M, ublas::upper> upper_part(const M& m) {
   return ublas::triangular_adaptor<const M, ublas::upper>( m );
}

template <typename T, typename W>
int do_memory_type(int n, W workspace) {
   typedef typename boost::numeric::bindings::traits::type_traits<T>::real_type real_type ;
   typedef std::complex< real_type >                                            complex_type ;

   typedef ublas::matrix<T, ublas::column_major> matrix_type ;
   typedef ublas::vector<T>                      vector_type ;

   // Set matrix
   matrix_type a( n, n );
   vector_type tau( n );

   randomize( a );
   matrix_type a2( a );

   // Compute QR factorization.
   lapack::geqrf( a, tau, workspace ) ;

   // Apply the orthogonal transformations to a2
   lapack::ormqr( 'L', transpose<T>::value, a, tau, a2, workspace );

   // The upper triangular parts of a and a2 must be equal.
   if (norm_frobenius( upper_part( a - a2 ) )
            > std::numeric_limits<real_type>::epsilon() * 10.0 * norm_frobenius( upper_part( a ) ) ) return 255 ;

   return 0 ;
} // do_value_type()



template <typename T>
int do_value_type() {
   const int n = 8 ;
   
   if (do_memory_type<T,lapack::optimal_workspace>( n, lapack::optimal_workspace() ) ) return 255 ;
   if (do_memory_type<T,lapack::minimal_workspace>( n, lapack::minimal_workspace() ) ) return 255 ;

   ublas::vector<T> work( n );
   do_memory_type<T, lapack::detail::workspace1<ublas::vector<T> > >( n, lapack::workspace(work) );
   return 0;
} // do_value_type()


int main() {
   // Run tests for different value_types
   if (do_value_type<float>()) return 255;
   if (do_value_type<double>()) return 255;
   if (do_value_type< std::complex<float> >()) return 255;
   if (do_value_type< std::complex<double> >()) return 255;

   std::cout << "Regression test succeeded\n" ;
   getchar();
   return 0;
}
#endif


#ifdef LTEST2

//  Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//  Copyright Toon Knapen, Karl Meerbergen

#include "random.hpp"

#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/bindings/traits/ublas_vector.hpp>
#include <boost/numeric/bindings/traits/ublas_matrix.hpp>
#include <boost/numeric/bindings/traits/std_vector.hpp>
#include <boost/numeric/bindings/blas/blas1.hpp>

#include <vector>
#include <complex>
#include <iostream>
#include <limits>
#include <cmath>



// Randomize a vector (using functions from random.hpp)
template <typename V>
void randomize(V& v) {
   for (typename V::size_type i=0; i<v.size(); ++i)
      v[i] = random_value< typename V::value_type >() ;
} // randomize()


float abs_sum_value( float const& f ) {
  using namespace std ;
  return abs(f) ;
}

double abs_sum_value( double const& f ) {
  using namespace std ;
  return abs(f) ;
}

float abs_sum_value( std::complex< float > const& f ) {
  using namespace std ;
  return abs(f.real()) + abs(f.imag()) ;
}

double abs_sum_value( std::complex< double > const& f ) {
  using namespace std ;
  return abs(f.real()) + abs(f.imag()) ;
}

template <typename V>
typename boost::numeric::bindings::traits::type_traits<typename V::value_type>::real_type abs_sum( V const& v) {
  typedef typename boost::numeric::bindings::traits::type_traits<typename V::value_type>::real_type real_type ;

  real_type sum( 0.0 ) ;
  for ( typename V::size_type i=0; i<v.size(); ++i ) {
    sum += abs_sum_value( v[i] ) ;
  }
  return sum ;
}


// Blas operations using one vector.
template <typename T>
struct OneVector {
  boost::numeric::ublas::vector<T> v_ref_ ;

  // Initialize : set reference vector (ublas)
  OneVector()
  : v_ref_( 10 )
  {
     randomize(v_ref_);
  }

  template <typename V>
  int operator()(V& v) const {
     using namespace boost::numeric::bindings::blas ;

     typedef typename V::value_type                                                        value_type ;
     typedef typename boost::numeric::bindings::traits::type_traits<value_type>::real_type real_type ;

     // Copy vector from reference
     for (typename V::size_type i=0; i<v_ref_.size(); ++i)
        v[i] = v_ref_(i);

     // Test blas routines and compare with reference
     real_type nrm = nrm2( v );
     if ( std::abs(nrm - norm_2(v_ref_)) > std::numeric_limits< real_type >::epsilon() * norm_2(v_ref_)) {
       std::cout << "nrm2 : " << std::abs(nrm - norm_2(v_ref_)) << " > " << std::numeric_limits< real_type >::epsilon() * norm_2(v_ref_) << std::endl ;
       return 255 ;
     }

     nrm = asum( v );
     if ( std::abs(nrm - abs_sum(v_ref_)) > std::numeric_limits< real_type >::epsilon() * abs_sum(v_ref_)) {
       std::cout << "asum : " << std::abs(nrm - abs_sum(v_ref_)) << " > " << std::numeric_limits< real_type >::epsilon() * abs_sum(v_ref_) << std::endl ;
       return 255 ;
     }

     scal( value_type(2.0), v );
     for (typename V::size_type i=0; i<v_ref_.size(); ++i)
        if (std::abs( v[i] - real_type(2.0)*v_ref_(i) ) > real_type(2.0)*std::abs(v_ref_(i))) return 255 ;

     return 0;
  }

  // Return the size of a vector.
  size_t size() const {return v_ref_.size();}
};


// Operations with two vectors.
template <typename T, typename V>
struct BaseTwoVectorOperations {
  typedef T                                                                             value_type ;
  typedef typename boost::numeric::bindings::traits::type_traits<value_type>::real_type real_type ;
  typedef boost::numeric::ublas::vector<T>                                              ref_vector_type ;

  // Initialize: select the first vector and set the reference vectors (ublas)
  BaseTwoVectorOperations(V& v, const ref_vector_type& v1_ref, const ref_vector_type& v2_ref)
  : v_( v )
  , v1_ref_( v1_ref )
  , v2_ref_( v2_ref )
  {}

  // Copy the 2nd reference vector into w.
  template <typename W>
  void copy_vector(W& w) const {
     for (size_t i=0; i<size(); ++i) {
        w[i] = v2_ref_(i);
     }
  } // copy_vector()

  // Get the size of a vector.
  size_t size() const {return v_.size();}

  // Data members.
  V&                     v_ ;
  const ref_vector_type& v1_ref_, v2_ref_ ;
};


template <typename T, typename V>
struct TwoVectorOperations { } ;


template <typename V>
struct TwoVectorOperations< float, V>
: BaseTwoVectorOperations<float,V> {
  typedef typename V::value_type                                                        value_type ;
  typedef typename boost::numeric::bindings::traits::type_traits<value_type>::real_type real_type ;
  typedef typename BaseTwoVectorOperations<float,V>::ref_vector_type                    ref_vector_type ;

  TwoVectorOperations(V& v, const ref_vector_type& v1_ref, const ref_vector_type& v2_ref)
  : BaseTwoVectorOperations<float,V>( v, v1_ref, v2_ref )
  {}

  // Perform the tests of blas functions and compare with reference
  template <typename W>
  int operator()(W& w) const {
     using namespace boost::numeric::bindings::blas ;

     copy_vector(w);

     // Test blas routines
     value_type prod = dot( this->v_, w );
     if ( std::abs(prod - inner_prod( this->v1_ref_, this->v2_ref_ ))
          > std::numeric_limits< real_type >::epsilon() * std::abs(prod)) return 255 ;

     axpy( value_type(2.0), this->v_, w );
     for (size_t i=0; i<this->size(); ++i)
        if ( std::abs(w[i] - (this->v2_ref_(i) + value_type(2.0)*this->v1_ref_(i)))
          > std::numeric_limits< real_type >::epsilon() * std::abs(w[i])) return 255 ;

     scal( value_type(0.0), w ) ;
     copy( this->v_, w ) ;
     for (size_t i=0; i<this->size(); ++i) {
        if ( std::abs( w[i] - this->v_[i] ) != 0.0 ) return 255 ;
     }

     return 0;
  }
};


template <typename V>
struct TwoVectorOperations< double, V>
: BaseTwoVectorOperations<double,V> {
  typedef typename V::value_type                                                        value_type ;
  typedef typename boost::numeric::bindings::traits::type_traits<value_type>::real_type real_type ;
  typedef typename BaseTwoVectorOperations<double,V>::ref_vector_type                   ref_vector_type ;

  TwoVectorOperations(V& v, const ref_vector_type& v1_ref, const ref_vector_type& v2_ref)
  : BaseTwoVectorOperations<double,V>( v, v1_ref, v2_ref )
  {}

  // Perform the tests of blas functions and compare with reference
  template <typename W>
  int operator()(W& w) const {
     using namespace boost::numeric::bindings::blas ;

     copy_vector( w );

     // Test blas routines
     value_type prod = dot( this->v_, w );
     if ( std::abs(prod - inner_prod( this->v1_ref_, this->v2_ref_ ))
          > std::numeric_limits< real_type >::epsilon() * std::abs(prod)) return 255 ;

     axpy( value_type(2.0), this->v_, w );
     for (size_t i=0; i<this->size(); ++i)
        if ( std::abs(w[i] - (this->v2_ref_(i) + value_type(2.0)*this->v1_ref_(i)))
          > std::numeric_limits< real_type >::epsilon() * std::abs(w[i])) return 255 ;

     copy_vector( w ) ;
     scal( value_type(-1.0), w ) ;
     ::boost::numeric::bindings::blas::copy( this->v_, w ) ;
     for (size_t i=0; i<this->size(); ++i) {
        if ( w[i] != this->v_[i] ) return 255 ;
     }

     return 0;
  }
};


template <typename V>
struct TwoVectorOperations< std::complex<float>, V>
: BaseTwoVectorOperations< std::complex<float>, V>
{
  typedef typename V::value_type                                                        value_type ;
  typedef typename boost::numeric::bindings::traits::type_traits<value_type>::real_type real_type ;
  typedef typename BaseTwoVectorOperations<std::complex<float>,V>::ref_vector_type      ref_vector_type ;

  TwoVectorOperations(V& v, const ref_vector_type& v1_ref, const ref_vector_type& v2_ref)
  : BaseTwoVectorOperations< std::complex<float>, V>( v, v1_ref, v2_ref )
  {}

  // Perform the tests of blas functions and compare with reference
  template <typename W>
  int operator()(W& w) const {
     using namespace boost::numeric::bindings::blas ;

     copy_vector( w );

     // Test blas routines
     value_type prod = dotc( this->v_, w );
     if ( std::abs(prod - inner_prod( conj(this->v1_ref_), this->v2_ref_ ))
          > std::numeric_limits< real_type >::epsilon() * std::abs(prod)) return 255 ;

     prod = dotu( this->v_, w );
     if ( std::abs(prod - inner_prod( this->v1_ref_, this->v2_ref_ ))
          > std::numeric_limits< real_type >::epsilon() * std::abs(prod)) return 255 ;

     axpy( value_type(2.0), this->v_, w );
     for (size_t i=0; i<this->size(); ++i)
        if ( std::abs(w[i] - (this->v2_ref_(i) + value_type(2.0)*this->v1_ref_(i)))
          > std::numeric_limits< real_type >::epsilon() * std::abs(w[i])) return 255 ;

     scal( value_type(0.0), w ) ;
     copy( this->v_, w ) ;
     for (size_t i=0; i<this->size(); ++i) {
        if ( std::abs( w[i] - this->v_[i] ) != 0.0 ) return 255 ;
     }

     return 0;
  }
};


template <typename V>
struct TwoVectorOperations< std::complex<double>, V>
: BaseTwoVectorOperations< std::complex<double>, V>
{
  typedef typename V::value_type                                                        value_type ;
  typedef typename boost::numeric::bindings::traits::type_traits<value_type>::real_type real_type ;
  typedef typename BaseTwoVectorOperations<std::complex<double>,V>::ref_vector_type     ref_vector_type ;

  TwoVectorOperations(V& v, const ref_vector_type& v1_ref, const ref_vector_type& v2_ref)
  : BaseTwoVectorOperations< std::complex<double>, V>( v, v1_ref, v2_ref )
  {}

  // Perform the tests of blas functions and compare with reference
  template <typename W>
  int operator()(W& w) const {
     using namespace boost::numeric::bindings::blas ;

     copy_vector( w );

     // Test blas routines
     value_type prod = dotc( this->v_, w );
     if ( std::abs(prod - inner_prod( conj(this->v1_ref_), this->v2_ref_ ))
          > std::numeric_limits< real_type >::epsilon() * std::abs(prod)) return 255 ;

     prod = dotu( this->v_, w );
     if ( std::abs(prod - inner_prod( this->v1_ref_, this->v2_ref_ ))
          > std::numeric_limits< real_type >::epsilon() * std::abs(prod)) return 255 ;

     axpy( value_type(2.0), this->v_, w );
     for (size_t i=0; i<this->size(); ++i)
        if ( std::abs(w[i] - (this->v2_ref_(i) + value_type(2.0)*this->v1_ref_(i)))
          > std::numeric_limits< real_type >::epsilon() * std::abs(w[i])) return 255 ;

     scal( value_type(0.0), w ) ;
     copy( this->v_, w ) ;
     for (size_t i=0; i<this->size(); ++i) {
        if ( std::abs( w[i] - this->v_[i] ) != 0.0 ) return 255 ;
     }

     return 0;
  }
};


// Run the tests for different types of vectors.
template <typename T, typename F>
int different_vectors(const F& f) {
   // Do test for different types of vectors
   {
      std::cout << "  ublas::vector\n" ;
      boost::numeric::ublas::vector< T > v(f.size());
      if (f( v )) return 255 ;
   }
   { 
      std::cout << "  std::vector\n" ;
      std::vector<T> v_ref(f.size());
      if (f( v_ref )) return 255 ;
   }
   {
      std::cout << "  ublas::vector_range\n" ;
      typedef boost::numeric::ublas::vector< T > vector_type ;
      vector_type v(f.size()*2);
      boost::numeric::ublas::vector_range< vector_type > vr(v, boost::numeric::ublas::range(1,1+f.size()));
      if (f( vr )) return 255 ;
   }
   {
      typedef boost::numeric::ublas::matrix< T, boost::numeric::ublas::column_major >  matrix_type ;
      matrix_type  m(f.size(),f.size()) ;

      std::cout << "  ublas::matrix_column\n" ;
      boost::numeric::ublas::matrix_column< matrix_type > m_c( m, 2 );
      if (f( m_c )) return 255 ;

      std::cout << "  ublas::matrix_row\n" ;
      boost::numeric::ublas::matrix_row< matrix_type > m_r( m, 1 );
      if (f( m_r )) return 255 ;
   }
   return 0;
} // different_vectors()


// This is the functor that selects the first vector of the tests that use two vectors.
template <typename T>
struct TwoVector {
   TwoVector()
   : v1_ref_( 10 )
   , v2_ref_( 10 )
   {}

   template <typename V>
   int operator() (V& v) const {
      for (size_t i=0; i<size(); ++i) v[i] = v1_ref_(i) ;
      return different_vectors<T,TwoVectorOperations<T,V> >( TwoVectorOperations<T,V>(v, v1_ref_, v2_ref_) ) ;
   }

   size_t size() const {
      return v1_ref_.size() ;
   }

   boost::numeric::ublas::vector<T> v1_ref_ ;
   boost::numeric::ublas::vector<T> v2_ref_ ;
}; // TwoVector


// Run the test for a specific value_type T.
template <typename T>
int do_value_type() {
   // Tests for functions with one vector argument.
   std::cout << " one argument\n";
   if (different_vectors<T,OneVector<T> >(OneVector<T> ())) return 255 ;

   // Tests for functions with two vector arguments.
   std::cout << " two arguments\n";
   if (different_vectors<T,TwoVector<T> >(TwoVector<T>())) return 255;
   return 0;
} // do_value_type()


int main() {
  // Run regression for Real/Complex
  std::cout << "float\n"; if (do_value_type<float>() ) return 255 ;
  std::cout << "double\n"; if (do_value_type<double>() ) return 255 ;
  std::cout << "complex<float>\n"; if (do_value_type<std::complex<float> >() ) return 255 ;
  std::cout << "complex<double>\n"; if (do_value_type<std::complex<double> >() ) return 255 ;

  std::cout << "Regression test successful\n" ;

  return 0 ;
}

#endif

#ifdef LTEST4
//  Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//  Copyright Toon Knapen, Karl Meerbergen

#include "random.hpp"

#include <boost/numeric/bindings/traits/transpose.hpp>
#include <boost/numeric/bindings/traits/ublas_vector.hpp>
#include <boost/numeric/bindings/traits/ublas_matrix.hpp>
#include <boost/numeric/bindings/traits/ublas_symmetric.hpp>
#include <boost/numeric/bindings/traits/ublas_hermitian.hpp>
#include <boost/numeric/bindings/blas/blas3.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/symmetric.hpp>
#include <boost/numeric/ublas/hermitian.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <iostream>
#include <complex>


// Randomize a matrix
template <typename M>
void randomize(M& m) {
   typedef typename M::size_type  size_type ;
   typedef typename M::value_type value_type ;

   size_type size1 = m.size1() ;
   size_type size2 = m.size2() ;

   for (size_type i=0; i<size2; ++i) {
      for (size_type j=0; j<size1; ++j) {
         m(j,i) = random_value< value_type >() ;
      }
   }
} // randomize()


template <typename T>
struct RealDiagonal {
   template <typename M>
   void operator() (M& m) const {}
};


template <typename T>
struct RealDiagonal< std::complex<T> > {
   template <typename M>
   void operator() (M& m) const {
      for (typename M::size_type i=0; i<m.size1(); ++i) {
         m(i,i) = m(i,i).real();
      }
   }
};


template <typename M>
boost::numeric::ublas::triangular_adaptor<const M, boost::numeric::ublas::unit_upper> unit_upper_part(const M& m) {
   return boost::numeric::ublas::triangular_adaptor<const M, boost::numeric::ublas::unit_upper> (m);
}

template <typename M>
boost::numeric::ublas::triangular_adaptor<const M, boost::numeric::ublas::upper> upper_part(const M& m) {
   return boost::numeric::ublas::triangular_adaptor<const M, boost::numeric::ublas::upper> (m);
}

template <typename M>
boost::numeric::ublas::triangular_adaptor<const M, boost::numeric::ublas::lower> lower_part(const M& m) {
   return boost::numeric::ublas::triangular_adaptor<const M, boost::numeric::ublas::lower> (m);
}


// Run over all matrix types and aply a functor to it.
template <typename F>
int do_matrix_types(const F& f) {
   using namespace boost::numeric::ublas ;

   typedef typename F::value_type value_type ;

   {
      matrix<value_type,column_major> m( f.size1(), f.size2() );
      if (f(m)) return 255;
   }
   {
      typedef matrix<value_type,column_major> matrix_type ;
      matrix_type m( 2*f.size1(), 2*f.size2() );
      matrix_range<matrix_type> m_r( m, range(1,1+f.size1()), range(2,2+f.size2()) );
      if (f(m_r)) return 255 ;
   }

   return 0;
} // do_matrix_types()


// Functor to select syrk/herk calls given the two matrix_types
template <typename M1>
struct Syrk2 {
   typedef typename M1::value_type                                                         value_type ;
   typedef boost::numeric::ublas::matrix<value_type, boost::numeric::ublas::column_major>  ref_matrix_type ;
   typedef typename boost::numeric::bindings::traits::type_traits< value_type >::real_type real_type ;
   typedef typename ref_matrix_type::size_type                                             size_type ;

   Syrk2(const M1& a,
        const ref_matrix_type& a_ref)
   : a_( a ), a_ref_( a_ref )
   , c_ref_( a_ref_.size1(), a_ref_.size1() )
   {
      randomize( c_ref_ );
      RealDiagonal<value_type>() (c_ref_); // Must have real diagonal for herk.
   }

   template <typename M>
   int operator() (M& c) const {
      using namespace boost::numeric::ublas ;

      real_type alpha = 2.0 ;
      real_type beta = -3.0 ;

      c.assign( c_ref_ );
      boost::numeric::bindings::blas::syrk( 'U', 'N', value_type(alpha), a_, value_type(beta), c ) ;
      if ( norm_frobenius( upper_part( c - (beta*c_ref_ + alpha * prod( a_ref_, trans( a_ref_ ) ) ) ) )
          > std::numeric_limits< real_type >::epsilon() * norm_frobenius( upper_part(c) ) ) return 255;

      c.assign( c_ref_ );
      symmetric_adaptor<M, upper> c_s( c );
      boost::numeric::bindings::blas::syrk( 'U', 'N', value_type(alpha), a_, value_type(beta), c_s ) ;
      if ( norm_frobenius( upper_part( c_s - (beta*c_ref_ + alpha * prod( a_ref_, trans( a_ref_ ) ) ) ) )
          > std::numeric_limits< real_type >::epsilon() * norm_frobenius( upper_part(c_s) ) ) return 255;

      c.assign( c_ref_ );
      boost::numeric::bindings::blas::herk( 'U', 'N', alpha, a_, beta, c ) ;
      if ( norm_frobenius( upper_part( c - (beta*c_ref_ + alpha * prod( a_ref_, herm( a_ref_ ) ) ) ) )
          > std::numeric_limits< real_type >::epsilon() * norm_frobenius( upper_part(c) ) ) return 255;

      c.assign( c_ref_ );
      hermitian_adaptor<M, upper> c_h( c );
      boost::numeric::bindings::blas::herk( 'U', 'N', alpha, a_, beta, c_h ) ;
      if ( norm_frobenius( upper_part( c_h - (beta*c_ref_ + alpha * prod( a_ref_, herm( a_ref_ ) ) ) ) )
          > std::numeric_limits< real_type >::epsilon() * norm_frobenius( upper_part(c_h) ) ) return 255;

      return 0;
   }

   size_type size1() const {return c_ref_.size1() ;}
   size_type size2() const {return c_ref_.size2() ;}

   const M1& a_ ;
   const ref_matrix_type& a_ref_;
   ref_matrix_type  c_ref_;
}; // Syrk2


template <typename T>
struct Syrk1 {
   typedef T                                                                               value_type ;
   typedef boost::numeric::ublas::matrix<value_type, boost::numeric::ublas::column_major>  ref_matrix_type ;
   typedef typename boost::numeric::bindings::traits::type_traits< value_type >::real_type real_type ;
   typedef typename ref_matrix_type::size_type                                             size_type ;

   Syrk1()
   : a_ref_( 4, 7 )
   {
      randomize( a_ref_ );
   }

   template <typename M>
   int operator() (M& a) const {
      typedef Syrk2<M> functor_type ;
      a.assign( a_ref_ );
      return do_matrix_types( functor_type(a, a_ref_) );
   }

   size_type size1() const {return a_ref_.size1();}
   size_type size2() const {return a_ref_.size2();}

   ref_matrix_type a_ref_;
}; // Syrk1


// Functor to select gemm calls given the three matrix_types
template <typename M1, typename M2>
struct Gemm3 {
   typedef typename M1::value_type                                                         value_type ;
   typedef boost::numeric::ublas::matrix<value_type, boost::numeric::ublas::column_major>  ref_matrix_type ;
   typedef typename boost::numeric::bindings::traits::type_traits< value_type >::real_type real_type ;
   typedef typename ref_matrix_type::size_type                                             size_type ;

   Gemm3(const M1& a, const M2& b,
        const ref_matrix_type& a_ref, const ref_matrix_type& b_ref)
   : a_( a ), b_( b )
   , a_ref_( a_ref ), b_ref_( b_ref ), c_ref_( a_ref_.size1(), b_ref_.size2() )
   {
      randomize( c_ref_ );
   }

   template <typename M>
   int operator() (M& c) const {
      c.assign( c_ref_ );

      value_type alpha = 2.0 ;
      value_type beta = -3.0 ;

      boost::numeric::bindings::blas::gemm( 'N', 'N', alpha, a_, b_, beta, c ) ;
      if ( norm_frobenius( c - (beta*c_ref_ + alpha * prod( a_ref_, b_ref_ ) ) )
          > std::numeric_limits< real_type >::epsilon() * norm_frobenius( c ) ) return 255;

      return 0;
   }

   size_type size1() const {return c_ref_.size1() ;}
   size_type size2() const {return c_ref_.size2() ;}

   const M1& a_ ;
   const M2& b_ ;
   const ref_matrix_type& a_ref_, b_ref_;
   ref_matrix_type  c_ref_;
}; // Gemm3


template <typename M1>
struct Gemm2 {
   typedef typename M1::value_type                                                         value_type ;
   typedef boost::numeric::ublas::matrix<value_type, boost::numeric::ublas::column_major>  ref_matrix_type ;
   typedef typename boost::numeric::bindings::traits::type_traits< value_type >::real_type real_type ;
   typedef typename ref_matrix_type::size_type                                             size_type ;

   Gemm2(const M1& a, const ref_matrix_type& a_ref)
   : a_( a )
   , a_ref_( a_ref )
   , b_ref_( a_ref.size2(), 7 )
   {
      randomize( b_ref_ );
   }

   template <typename M>
   int operator() (M& b) const {
      typedef Gemm3<M1,M> functor_type ;
      b.assign( b_ref_ );
      return do_matrix_types( functor_type(a_, b, a_ref_, b_ref_) );
   }

   size_type size1() const {return b_ref_.size1();}
   size_type size2() const {return b_ref_.size2();}

   const M1&              a_ ;
   const ref_matrix_type& a_ref_;
   ref_matrix_type        b_ref_;
}; // Gemm2


template <typename T>
struct Gemm1 {
   typedef T                                                                               value_type ;
   typedef boost::numeric::ublas::matrix<value_type, boost::numeric::ublas::column_major>  ref_matrix_type ;
   typedef typename boost::numeric::bindings::traits::type_traits< value_type >::real_type real_type ;
   typedef typename ref_matrix_type::size_type                                             size_type ;

   Gemm1()
   : a_ref_( 4, 7 )
   {
      randomize( a_ref_ );
   }

   template <typename M>
   int operator() (M& a) const {
      typedef Gemm2<M> functor_type ;
      a.assign( a_ref_ );
      return do_matrix_types( functor_type(a, a_ref_) );
   }

   size_type size1() const {return a_ref_.size1();}
   size_type size2() const {return a_ref_.size2();}

   ref_matrix_type a_ref_;
}; // Gemm1


// Functor to select syrk/herk calls given the two matrix_types
template <typename M1>
struct Trsm2 {
   typedef typename M1::value_type                                                         value_type ;
   typedef boost::numeric::ublas::matrix<value_type, boost::numeric::ublas::column_major>  ref_matrix_type ;
   typedef typename boost::numeric::bindings::traits::type_traits< value_type >::real_type real_type ;
   typedef typename ref_matrix_type::size_type                                             size_type ;

   Trsm2(M1& c,
        const ref_matrix_type& b_ref, const ref_matrix_type& c_ref )
   : c_( c ), c_ref_( c_ref ), b_ref_( b_ref )
   , a_ref_( b_ref_.size1(), c_ref_.size2() )
   {
      randomize( a_ref_ ) ;
   }

   template <typename M>
   int operator() (M& a) const {
      using namespace boost::numeric::ublas ;

      real_type alpha = 2.0 ;

      a.assign( a_ref_ );
      boost::numeric::bindings::blas::trsm( 'L', 'U', 'N', 'N', value_type(alpha), b_ref_, a ) ;
      if ( norm_frobenius( alpha * a_ref_ - prod( upper_part(b_ref_), a ) )
          > std::numeric_limits< real_type >::epsilon() * norm_frobenius( upper_part(b_ref_) ) * norm_frobenius( a) ) return 255;

      a.assign( a_ref_ );
      boost::numeric::bindings::blas::trsm( 'R', 'U', 'N', 'N', value_type(alpha), c_, a ) ;
      if ( norm_frobenius( alpha * a_ref_ - prod( a, upper_part(c_) ) )
          > std::numeric_limits< real_type >::epsilon() * norm_frobenius( upper_part(c_) ) * norm_frobenius( a ) ) return 255;

      a.assign( a_ref_ );
      boost::numeric::bindings::blas::trsm( 'R', 'L', 'N', 'N', value_type(alpha), c_, a ) ;
      if ( norm_frobenius( alpha * a_ref_ - prod( a, lower_part(c_) ) )
          > std::numeric_limits< real_type >::epsilon() * norm_frobenius( lower_part(c_) ) * norm_frobenius( a ) ) return 255;

      a.assign( a_ref_ );
      boost::numeric::bindings::blas::trsm( 'L', 'U', 'T', 'N', value_type(alpha), b_ref_, a ) ;
      if ( norm_frobenius( alpha * a_ref_ - prod( trans(upper_part(b_ref_)), a ) )
          > std::numeric_limits< real_type >::epsilon() * norm_frobenius( upper_part(b_ref_) ) * norm_frobenius( a) ) return 255;

      a.assign( a_ref_ );
      boost::numeric::bindings::blas::trsm( 'L', 'U', 'N', 'U', value_type(alpha), b_ref_, a ) ;
      if ( norm_frobenius( alpha * a_ref_ - prod( unit_upper_part(b_ref_), a ) )
          > std::numeric_limits< real_type >::epsilon() * norm_frobenius( unit_upper_part(b_ref_) ) * norm_frobenius( a) ) return 255;

      return 0;
   }

   size_type size1() const {return a_ref_.size1() ;}
   size_type size2() const {return a_ref_.size2() ;}

   const M1& c_ ;
   const ref_matrix_type& c_ref_;
   const ref_matrix_type& b_ref_;
   ref_matrix_type        a_ref_;
} ; // Trsm2


template <typename T>
struct Trsm1 {
   typedef T                                                                               value_type ;
   typedef boost::numeric::ublas::matrix<value_type, boost::numeric::ublas::column_major>  ref_matrix_type ;
   typedef typename boost::numeric::bindings::traits::type_traits< value_type >::real_type real_type ;
   typedef typename ref_matrix_type::size_type                                             size_type ;

   Trsm1()
   : b_( 4, 4 )
   , c_( 7, 7 )
   {
      randomize( b_ );
      randomize( c_ );
      for ( std::size_t i=0; i<b_.size1(); ++i ) if ( b_(i,i)==value_type(0.0) ) b_(i,i) = value_type(1.0) ;
      for ( std::size_t i=0; i<c_.size1(); ++i ) if ( c_(i,i)==value_type(0.0) ) c_(i,i) = value_type(1.0) ;
   }

   template <typename M>
   int operator() (M& c) const {
      typedef Trsm2<M> functor_type ;
      return do_matrix_types( functor_type(c, b_, c_) );
   }

   size_type size1() const {return c_.size1();}
   size_type size2() const {return c_.size2();}

   ref_matrix_type b_;
   ref_matrix_type c_;
} ; // Trsm1



template <typename T>
int do_value_type() {
   // Gemm test
   if (do_matrix_types( Gemm1<T>() )) return 255 ;

   // Syrk and herk test
   if (do_matrix_types( Syrk1<T>() )) return 255 ;

   // Trsm test
   if (do_matrix_types( Trsm1<T>() )) return 255 ;

   return 0 ;
} // do_value_type()


int main() {
   // Test for various value_types
   if (do_value_type<float>()) return 255 ;
   if (do_value_type<double>()) return 255 ;
   if (do_value_type<std::complex<float> >()) return 255 ;
   if (do_value_type<std::complex<double> >()) return 255 ;

   std::cout << "Regression test succeeded\n" ;

   return 0 ;
}

#endif //LTEST4

#ifdef LTEST5

#include <cstddef>
#include <iostream>
#include <algorithm> 
#include <boost/numeric/bindings/lapack/gesvd.hpp>
#include <boost/numeric/bindings/traits/ublas_matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/bindings/traits/ublas_vector.hpp>
#include <boost/numeric/bindings/traits/std_vector.hpp>
#include "utils.h"

namespace ublas = boost::numeric::ublas;
namespace lapack = boost::numeric::bindings::lapack;

using std::size_t; 
using std::cout;
using std::endl; 

typedef double real_t; 
typedef ublas::matrix<real_t, ublas::column_major> m_t;
typedef ublas::vector<real_t> v_t;

int main() {

  cout << endl; 

  size_t m = 3, n = 4;   
  size_t minmn = m < n ? m : n; 
  m_t a (m, n);  
  a(0,0) = 2.;  a(0,1) = 2.;  a(0,2) = 2.;   a(0,3) = 2.;
  a(1,0) = 1.7; a(1,1) = 0.1; a(1,2) = -1.7; a(1,3) = -0.1;
  a(2,0) = 0.6; a(2,1) = 1.8; a(2,2) = -0.6; a(2,3) = -1.8;

  m_t a2 (a); // for parts 2 & 3

  print_m (a, "A"); 
  cout << endl; 

  v_t s (minmn); 
  m_t u (m, minmn);
  m_t vt (minmn, n);

  lapack::gesvd (a, s, u, vt);  

  print_v (s, "s"); 
  cout << endl; 
  print_m (u, "U"); 
  cout << endl; 
  print_m (vt, "V^T"); 
  cout << endl << endl;

  // part 2

  // singular values and singular vectors satisfy  A v_i == s_i u_i
  for (int i = 0; i < minmn; ++i) {
    cout << "A v_" << i << " == s_" << i << " u_" << i << endl; 
	v_t avi = ublas::prod (a2, ublas::row (vt, i));  
    print_v (avi);
    v_t siui = s[i] * ublas::column (u, i);
    print_v (siui);
    cout << endl; 
  }
  cout << endl; 
  
  // singular values and singular vectors satisfy  A^T u_i == s_i v_i
  for (int i = 0; i < minmn; ++i) {
    cout << "A^T u_" << i << " == s_" << i << " v_" << i << endl; 
    v_t atui = ublas::prod (trans (a2), ublas::column (u, i));  
    print_v (atui);
    v_t sivi = s[i] * ublas::row (vt, i);
    print_v (sivi);
    cout << endl; 
  }
  cout << endl; 
  
  // part 3 
  
  lapack::gesvd (a2, s);  
  
  print_v (s, "singular values only"); 
  cout << endl; 
}
#endif

#ifdef LTEST6

//#define BOOST_NUMERIC_BINDINGS_LAPACK_2

#include <cstddef>
#include <iostream>
#include <algorithm> 
#include <boost/numeric/bindings/lapack/gesvd.hpp>
#include <boost/numeric/bindings/traits/ublas_matrix.hpp>
#include <boost/numeric/bindings/traits/ublas_vector.hpp>
#include <boost/numeric/bindings/traits/std_vector.hpp>
#include "utils.h"

namespace ublas = boost::numeric::ublas;
namespace lapack = boost::numeric::bindings::lapack;

using std::size_t; 
using std::cout;
using std::endl; 

typedef double real_t; 
typedef ublas::matrix<real_t, ublas::column_major> m_t;
typedef ublas::vector<real_t> v_t;

int main() {

  cout << endl; 

  size_t m = 3, n = 2;   
  size_t minmn = m < n ? m : n; 
  m_t a (m, n);  
  a(0,0) = 1.; a(0,1) = 1.;
  a(1,0) = 0.; a(1,1) = 1.;
  a(2,0) = 1.; a(2,1) = 0.;

  m_t a2 (a); // for part 2
  m_t a3 (a); // for part 3

  print_m (a, "A"); 
  cout << endl; 

  v_t s (minmn);
  m_t u (m, m);
  m_t vt (n, n);

  size_t lw; 

#ifndef BOOST_NUMERIC_BINDINGS_LAPACK_2
  lw = lapack::gesvd_work ('O', 'N', 'N', a); 
  cout << "opt NN lw: " << lw << endl; 
  lw = lapack::gesvd_work ('O', 'A', 'A', a); 
  cout << "opt AA lw: " << lw << endl; 
  lw = lapack::gesvd_work ('O', 'S', 'S', a); 
  cout << "opt SS lw: " << lw << endl; 
  lw = lapack::gesvd_work ('O', 'O', 'N', a); 
  cout << "opt ON lw: " << lw << endl; 
  lw = lapack::gesvd_work ('O', 'N', 'O', a); 
  cout << "opt NO lw: " << lw << endl; 
#endif 
  lw = lapack::gesvd_work ('M', 'A', 'A', a); 
  cout << "min lw: " << lw << endl << endl; 

#ifndef BOOST_NUMERIC_BINDINGS_LAPACK_2
  lw = lapack::gesvd_work ('O', 'A', 'A', a); 
#endif 

  std::vector<real_t> w (lw); 

  lapack::gesvd ('A', 'A', a, s, u, vt, w);  

  print_v (s, "s"); 
  cout << endl; 
  print_m (u, "U"); 
  cout << endl; 
  print_m (vt, "V^T"); 
  cout << endl; 

  m_t sm (m, n); 
  for (size_t i = 0; i < s.size(); ++i) 
    sm (i,i) = s (i); 
  print_m (sm, "S"); 
  cout << endl;

  a = ublas::prod (u, m_t (ublas::prod (sm, vt))); 
  print_m (a, "A == U S V^T"); 
  cout << endl; 

  // part 2 

  cout << endl << "part 2" << endl << endl; 
 
#ifndef BOOST_NUMERIC_BINDINGS_LAPACK_2
  lapack::gesvd ('A', 'A', a2, s, u, vt);  
#else
  lapack::gesvd ('M', 'A', 'A', a2, s, u, vt);  
#endif

  print_v (s, "s"); 
  cout << endl; 
  print_m (u, "U"); 
  cout << endl; 
  print_m (vt, "V^T"); 
  cout << endl; 

  for (size_t i = 0; i < s.size(); ++i) 
    sm (i,i) = s (i); 
  print_m (sm, "S"); 
  cout << endl;

  a2 = ublas::prod (u, m_t (ublas::prod (sm, vt))); 
  print_m (a2, "A == U S V^T"); 
  cout << endl; 
 
  // part 3

  cout << endl << "part 3" << endl << endl; 

#ifndef BOOST_NUMERIC_BINDINGS_LAPACK_2
  cout << "opt lw: " << lapack::gesvd_work ('O', 'N', 'N', a3) 
       << endl << endl; 
  lapack::gesvd (a3, s);
#else 
  cout << "min lw: " << lapack::gesvd_work ('M', 'N', 'N', a3) 
       << endl << endl; 
  lapack::gesvd ('M', 'N', 'N', a3, s, u, vt);
#endif 

  print_v (s, "singular values only"); 
  cout << endl; 

  cout << endl; 

}


#endif