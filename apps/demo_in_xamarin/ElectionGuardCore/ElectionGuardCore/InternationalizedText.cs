using System.Collections.Generic;
using System.Linq;

namespace ElectionGuardCore
{
    public class InternationalizedText
    {
        public List<LanguageText> Text { get; set; }

        public string GetTextValue(string language)
        {
            return Text.FirstOrDefault(t => t.Language == language)?.Value;
        }
    }
}
