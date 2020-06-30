//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "ADMaterial.h"

class INSADObjectTracker;

class INSADMaterial : public ADMaterial
{
public:
  static InputParameters validParams();

  INSADMaterial(const InputParameters & parameters);

  void initialSetup() override;

protected:
  virtual void computeQpProperties() override;

  /**
   * compute the strong form corresponding to RZ pieces of the viscous term
   */
  void viscousTermRZ();

  /// velocity
  const ADVectorVariableValue & _velocity;

  /// gradient of velocity
  const ADVectorVariableGradient & _grad_velocity;

  /// gradient of the pressure
  const ADVariableGradient & _grad_p;

  /// viscosity
  const ADMaterialProperty<Real> & _mu;

  /// density
  const ADMaterialProperty<Real> & _rho;

  /// Time derivative of the velocity, e.g. the acceleration
  const ADVectorVariableValue * _velocity_dot;

  /// The strong residual of the mass continuity equation
  ADMaterialProperty<Real> & _mass_strong_residual;

  /// Strong residual corresponding to the momentum advective term
  ADMaterialProperty<RealVectorValue> & _advective_strong_residual;

  /// Strong residual corresponding to the momentum viscous term. This is only used by stabilization
  /// kernels
  ADMaterialProperty<RealVectorValue> & _viscous_strong_residual;

  /// Strong residual corresponding to the momentum transient term
  ADMaterialProperty<RealVectorValue> & _td_strong_residual;

  /// Strong residual corresponding to the momentum gravity term
  ADMaterialProperty<RealVectorValue> & _gravity_strong_residual;

  /// Strong residual corresponding to the momentum boussinesq term
  ADMaterialProperty<RealVectorValue> & _boussinesq_strong_residual;

  /// Strong residual corresponding to coupled force term
  ADMaterialProperty<RealVectorValue> & _coupled_force_strong_residual;

  // /// Future addition pending addition of INSADMMSKernel.
  // /// Strong residual corresponding to the mms function term
  // MaterialProperty<RealVectorValue> & _mms_function_strong_residual;

  /// Whether we are on the displaced mesh
  const bool _use_displaced_mesh;

  /// The quadrature points with potential partial derivatives with respect to displacement degrees
  /// of freedom
  const MooseArray<ADPoint> & _ad_q_point;

  /// A user object that consumes information from INSAD residual objects and feeds it into this
  /// material
  static const INSADObjectTracker * _object_tracker;

  /// Whether the simulation is transient
  bool _has_transient;

  /// Whether there is a gravity force in the momentum equation
  bool _has_gravity;

  /// Whether natural convection forces via the Boussinesq approximation are added to the momentum
  /// equation
  bool _has_boussinesq;

  /// Whether there is a force from a coupled vector variable or vector function
  bool _has_coupled_force;

  /// The Boussinesq coefficient
  const ADMaterialProperty<Real> * _boussinesq_alpha;

  /// The temperature
  const ADVariableValue * _temperature;

  /// The reference temperature
  const MaterialProperty<Real> * _ref_temp;

  /// The viscous form of the equations. This is either "laplace" or "traction"
  std::string _viscous_form;

  /// The gravity vector
  RealVectorValue _gravity_vector;

  /// An optionally copuled vector var
  const ADVectorVariableValue * _coupled_force_var;

  /// An optional vector function
  const Function * _coupled_force_vector_function;
};
