classdef dtCV
  properties (SetAccess = private, GetAccess = public)
    label_
    value_
    min_
    max_
  end
  methods
    function obj = dtCV(label, value, min, max)
      obj.label_ = label;
      obj.value_ = value;
      obj.min_ = min;
      obj.max_ = max;
    end         
  end
end