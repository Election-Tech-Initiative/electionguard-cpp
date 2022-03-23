using System;

namespace ElectionGuard
{
    /// <summary>
    /// A custom exception class to handle messages and the status code
    /// </summary>
    public class ElectionGuardException : Exception
    {
        public Status Status { get; set; }

        public ElectionGuardException() { }
        public ElectionGuardException(string message) : base(message) { }
        public ElectionGuardException(string message, Status code) : base(message)
        {
            this.Status = code;
        }
        public ElectionGuardException(string message, System.Exception inner) : base(message, inner) { }
    }

}