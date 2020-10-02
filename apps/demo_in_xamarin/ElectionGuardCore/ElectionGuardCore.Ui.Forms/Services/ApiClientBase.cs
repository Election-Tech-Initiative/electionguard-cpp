using System;
using System.Collections.Generic;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;

namespace ElectionGuardCore.Ui.Forms.Services
{
    public abstract class ApiClientBase
    {
        private static readonly HttpClient Client = new HttpClient(); // instantiate once for app lifetime, per MS docs

        private readonly string _baseUri;

        protected ApiClientBase(string baseUri)
        {
            _baseUri = baseUri;
        }

        protected async Task SendRequest(string relativeUri, HttpMethod method,
            IDictionary<string, string> headers = null, object requestData = null)
        {
            using (var response = await SendHttpRequest(relativeUri, method, headers, requestData))
            {
                if (!response.IsSuccessStatusCode)
                {
                    ThrowOnNonSuccessStatusCode(response);
                }
            }
        }

        protected async Task<TResponse> SendRequestForResponse<TResponse>(string relativeUri, HttpMethod method,
            IDictionary<string, string> headers = null, object requestData = null)
        {
            using (var response = await SendHttpRequest(relativeUri, method, headers, requestData))
            {
                ThrowOnNonSuccessStatusCode(response);

                var responseJson = response.Content == null ? null : await response.Content.ReadAsStringAsync();
                return responseJson == null ? default : JsonConvert.DeserializeObject<TResponse>(responseJson);
            }
        }

        private async Task<HttpResponseMessage> SendHttpRequest(string relativeUri, HttpMethod method,
            IDictionary<string, string> headers, object requestData)
        {
            var requestJson = requestData == null ? null : JsonConvert.SerializeObject(requestData);

            using (var request = new HttpRequestMessage(method, GetUri(relativeUri)))
            {
                if (requestJson != null)
                {
                    request.Content = new StringContent(requestJson, Encoding.UTF8, "application/json");
                }

                if (headers != null)
                {
                    foreach (var header in headers)
                    {
                        request.Headers.Add(header.Key, header.Value);
                    }
                }

                return await Client.SendAsync(request);
            }
        }

        private string GetUri(string relativeUri)
        {
            return $"{_baseUri}{relativeUri}";
        }

        private void ThrowOnNonSuccessStatusCode(HttpResponseMessage response)
        {
            if (!response.IsSuccessStatusCode)
            {
                throw new Exception(); // TODO add better exception handling
            }
        }
    }
}
