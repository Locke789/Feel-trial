#include <feel/feel.hpp>

int main(int argc, char**argv )
{
    using namespace Feel;
      po::options_description laplacianoptions( "Laplacian options" );
      laplacianoptions.add_options()
      ( "mu", po::value<double>()->default_value( 1.0 ), "coeff" )
          ;

      Environment env( _argc=argc, _argv=argv,
                     _desc=laplacianoptions,
                     _about=about(_name="bubble_laplacian",
                                  _author="Feel++ Consortium",
                                  _email="feelpp-devel@feelpp.org"));

    auto mesh = loadMesh(_mesh=new Mesh<Simplex<2,1,FEELPP_CODIM>>);

    auto Vh = Pch<2>( mesh );
    auto u = Vh->element("u");
    auto v = Vh->element("v");
    auto f = expr( soption(_name="functions.f"), "f" );

    /// Initial state
    u.on(_range=elements(mesh), _expr=f);

    auto e = exporter( _mesh=mesh );

    /// RHS
    auto l = form1( _test=Vh );

    /// LHS
    auto a = form2( _trial=Vh, _test=Vh);

    /// Time Step
    auto mybdf = bdf(_space = Vh, _name="mybdf");
    mybdf->initialize( u );

    for(mybdf->start(); mybdf->isFinished() == false; mybdf->next(u))
    {
      if( Environment::isMasterRank() )
        std::cout << "Time = " << mybdf->time() << "s\n";
      l = integrate(_range=elements(mesh),_expr=idv(mybdf->polyDeriv())*id(u));
      a = integrate(_range=elements(mesh),_expr=mybdf->polyDerivCoefficient(0)*id(u)*idt(v) + doption("mu")*inner(grad(u),gradt(v)));
      if(FEELPP_CODIM == 2)
        a += on(_range=boundaryfaces(mesh),
            _expr=expr(soption("functions.g")),
            _rhs=l,
            _element=u);
      a.solve(_rhs=l,_solution=u);

      if(boption("exporter.export")){
        e->step(mybdf->time())->add( "u", u );
        e->save();
      }
    }
    return 0;
}
