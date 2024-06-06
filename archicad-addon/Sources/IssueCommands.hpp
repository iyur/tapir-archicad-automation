#pragma once

#include "CommandBase.hpp"

class GetIssueListCommand : public CommandBase
{
public:
    GetIssueListCommand ();
    virtual GS::String GetName () const override;
    virtual GS::Optional<GS::UniString> GetResponseSchema () const override;
    virtual GS::ObjectState Execute (const GS::ObjectState& parameters, GS::ProcessControl& processControl) const override;
};

class GetAttachedElementsCommand : public CommandBase
{
public:
    GetAttachedElementsCommand ();
    virtual GS::String GetName () const override;
    virtual GS::Optional<GS::UniString> GetResponseSchema () const override;
    virtual GS::ObjectState Execute (const GS::ObjectState& parameters, GS::ProcessControl& processControl) const override;
};

class ExportToBCFCommand : public CommandBase
{
public:
    ExportToBCFCommand ();
    virtual GS::String GetName () const override;
    virtual GS::ObjectState Execute (const GS::ObjectState& parameters, GS::ProcessControl& processControl) const override;
};

class ImportFromBCFCommand : public CommandBase
{
public:
    ImportFromBCFCommand ();
    virtual GS::String GetName () const override;
    virtual GS::ObjectState Execute (const GS::ObjectState& parameters, GS::ProcessControl& processControl) const override;
};