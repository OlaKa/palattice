template<typename VAL_T, typename MAP_T, typename IT_T>
void AccLatticeIterator<VAL_T,MAP_T,IT_T>::checkForEnd() const
{
  if (isEnd())
    throw palatticeError("Evaluation of lattice.end(), which is after last Element!");
}


// accessors

// position in Lattice in meter
template<typename VAL_T, typename MAP_T, typename IT_T>
double AccLatticeIterator<VAL_T,MAP_T,IT_T>::pos() const {
  checkForEnd(); 
  return it->first;
}

template<typename VAL_T, typename MAP_T, typename IT_T>
VAL_T AccLatticeIterator<VAL_T,MAP_T,IT_T>::element() const {
  checkForEnd();
  return it->second;
}


// iteration

template<typename VAL_T, typename MAP_T, typename IT_T>
AccLatticeIterator<VAL_T,MAP_T,IT_T>& AccLatticeIterator<VAL_T,MAP_T,IT_T>::next(element_type t, element_plane p, element_family f)
{
  ++it;
  for (; it!=latticeElements->end(); ++it) {
    if ( it->second->type==t && (p==noplane || it->second->plane==p) && (f==nofamily || it->second->family==f) )
      break;
  }
  return *this;
  //throw noMatchingElement("type, plane, family");
}

template<typename VAL_T, typename MAP_T, typename IT_T>
AccLatticeIterator<VAL_T,MAP_T,IT_T>& AccLatticeIterator<VAL_T,MAP_T,IT_T>::revolve()
{
  ++it;
  if (it == latticeElements->end()) {
    it = latticeElements->begin();
  }
  return *this;
}



// position calculations
template<typename VAL_T, typename MAP_T, typename IT_T>
double AccLatticeIterator<VAL_T,MAP_T,IT_T>::pos(Anchor anchor) const
{
  checkForEnd();

  if (anchor==*latticeRefPos)
    return it->first;

  double pos = it->first;
  double l = it->second->length;
  if (*latticeRefPos == Anchor::center) pos -= l/2;
  else if (*latticeRefPos == Anchor::end) pos -= l;

  switch(anchor) {
  case Anchor::begin:
    return pos;
  case Anchor::center:
    return pos + l/2;
  case Anchor::end:
    return pos + l;
  }

  return 0.;
}

template<typename VAL_T, typename MAP_T, typename IT_T>
bool AccLatticeIterator<VAL_T,MAP_T,IT_T>::at(double pos) const
{
  if (pos>=begin() && pos<=end())
    return true;
  else
    return false;
}

// distance from anchor of element to pos (>0 if pos is after anchor)
template<typename VAL_T, typename MAP_T, typename IT_T>
double AccLatticeIterator<VAL_T,MAP_T,IT_T>::distance(Anchor anchor, double pos) const
{
  return pos - this->pos(anchor);
}

// both directions are checked, shorter distance is returned.
template<typename VAL_T, typename MAP_T, typename IT_T>
double AccLatticeIterator<VAL_T,MAP_T,IT_T>::distanceRing(Anchor anchor, double pos) const
{
  double d_normal = distance(anchor,pos);
  double d_other = *latticeCircumference - abs(d_normal);
  if ( d_other >= 0.5 * *latticeCircumference ) // regular direction shorter
    return d_normal;
  else {
    if (d_normal>0)
      d_other *= -1;
    return d_other;
  }
}

// |distance| to next element (anchor to anchor, circulating)
template<typename VAL_T, typename MAP_T, typename IT_T>
double AccLatticeIterator<VAL_T,MAP_T,IT_T>::distanceNext(Anchor anchor) const
{
  auto nextIt = *this;
  nextIt.revolve();
  double d = distance(anchor,nextIt.pos(anchor));
  if (d < 0)
    d += *latticeCircumference;
  return d;
}


