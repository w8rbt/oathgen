#ifndef CRYPTOPP_ALTBASE32_H
#define CRYPTOPP_ALTBASE32_H

#include "basecode.h"

NAMESPACE_BEGIN(CryptoPP)

//! Converts given data to base 32, the alternate code is based on http://www.faqs.org/rfcs/rfc4648.html.
class AltBase32Encoder : public SimpleProxyFilter
{
public:
	AltBase32Encoder(BufferedTransformation *attachment = NULL, bool uppercase = true, int outputGroupSize = 0, const std::string &separator = ":", const std::string &terminator = "")
		: SimpleProxyFilter(new BaseN_Encoder(new Grouper), attachment)
	{
		IsolatedInitialize(MakeParameters(Name::Uppercase(), uppercase)(Name::GroupSize(), outputGroupSize)(Name::Separator(), ConstByteArrayParameter(separator)));
	}

	void IsolatedInitialize(const NameValuePairs &parameters);
};

//! Decode base 32 data back to bytes, the alternate code is based on http://www.faqs.org/rfcs/rfc4648.html.
class AltBase32Decoder : public BaseN_Decoder
{
public:
	AltBase32Decoder(BufferedTransformation *attachment = NULL)
		: BaseN_Decoder(GetDefaultDecodingLookupArray(), 5, attachment) {}

	void IsolatedInitialize(const NameValuePairs &parameters);

private:
	static const int * CRYPTOPP_API GetDefaultDecodingLookupArray();
};

NAMESPACE_END

#endif
