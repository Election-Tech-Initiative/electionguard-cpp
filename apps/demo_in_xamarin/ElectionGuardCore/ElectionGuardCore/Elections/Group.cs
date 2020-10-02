using System;
using System.Linq;
using System.Numerics;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System.Collections.Generic;
using System.Globalization;

namespace ElectionGuardCore.Elections
{
    /// <summary>
    /// An element of the larger `mod p` space, i.e., in [0, P), where P is a 4096-bit prime.
    /// </summary>
    [JsonConverter(typeof(ElementConverter))]
    public class ElementModP
    {
        public BigInteger Data { get; set; }
    }

    /// <summary>
    /// An element of the smaller `mod q` space, i.e., in [0, Q), where Q is a 256-bit prime.
    /// </summary>
    [JsonConverter(typeof(ElementConverter))]
    public class ElementModQ
    {
        public BigInteger Data { get; set; }
    }

    public class ElementConverter : JsonConverter
    {
        public override bool CanConvert(Type objectType)
        {
            return objectType == typeof(ElementModP) || objectType == typeof(ElementModQ);
        }
        public override object ReadJson(
            JsonReader reader, Type objectType, object existingValue, JsonSerializer serializer)
        {
            if (objectType == typeof(ElementModP))
            {
                return new ElementModP { Data = BigInteger.Parse((string)reader.Value, NumberStyles.AllowHexSpecifier) };
            }
            else
            {
                return new ElementModQ { Data = BigInteger.Parse((string)reader.Value, NumberStyles.AllowHexSpecifier) };
            }
        }

        public override void WriteJson(JsonWriter writer, object value, JsonSerializer serializer)
        {
            if (value.GetType() == typeof(ElementModP))
            {
                writer.WriteRawValue(((ElementModP)value).Data.ToString());
            }
            else
            {
                writer.WriteRawValue(((ElementModQ)value).Data.ToString());
            }
        }
    }
}
