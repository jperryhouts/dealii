// ---------------------------------------------------------------------
//
// Copyright (C) 2016 by the deal.II authors
//
// This file is part of the deal.II library.
//
// The deal.II library is free software; you can use it, redistribute
// it, and/or modify it under the terms of the GNU Lesser General
// Public License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// The full text of the license can be found in the file LICENSE at
// the top level of the deal.II distribution.
//
// ---------------------------------------------------------------------


// check that distributed_mg_dofs works correctly on non-standard oriented
// meshes when used in parallel

#include "../tests.h"
#include <deal.II/base/logstream.h>
#include <deal.II/base/mpi.h>
#include <deal.II/distributed/tria.h>
#include <deal.II/grid/grid_generator.h>
#include <deal.II/dofs/dof_handler.h>
#include <deal.II/fe/fe_q.h>


template <int dim>
void check()
{
  // need cubic polynomials that have two dofs on lines
  FE_Q<dim> fe(3);

  parallel::distributed::Triangulation<dim> tr(MPI_COMM_WORLD, Triangulation<dim>::none,
                                               parallel::distributed::Triangulation<dim>::construct_multigrid_hierarchy);
  GridGenerator::hyper_shell(tr, Point<dim>(), 0.5, 1, 12);
  tr.refine_global(1);

  DoFHandler<dim> dof(tr);
  dof.distribute_dofs(fe);
  dof.distribute_mg_dofs(fe);
  deallog << dim << "D OK" << std::endl;
}

int main(int argc, char **argv)
{
  Utilities::MPI::MPI_InitFinalize mpi(argc, argv);
  mpi_initlog();
  check<2> ();
  check<3> ();
}
