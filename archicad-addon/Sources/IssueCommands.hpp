#pragma once

#include "CommandBase.hpp"

class GetIssueCommand : public CommandBase
{
public:
    GetIssueCommand ();
    virtual GS::String GetName () const override;
    virtual GS::Optional<GS::UniString> GetResponseSchema () const override;
    virtual GS::ObjectState Execute (const GS::ObjectState& parameters, GS::ProcessControl& processControl) const override;
};