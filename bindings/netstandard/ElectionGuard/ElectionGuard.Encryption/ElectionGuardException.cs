using System;

namespace ElectionGuard
{
    /// <summary>
    /// A custom exception class to handle messages and the status code
    /// </summary>
    public class ElectionGuardException : Exception
    {
        /// <summary>
        /// Property for the status code of the exception
        /// </summary>
        public Status Status { get; set; }

        /// <summary>
        /// Default constructor
        /// </summary>
        public ElectionGuardException() { }

        /// <summary>
        /// Parameterized constructor that sets the message
        /// </summary>
        /// <param name="message">message to be used in the exception handling</param>
        /// <returns>New ElectionGuardException object</returns>
        public ElectionGuardException(string message) : base(message) { }

        /// <summary>
        /// Parameterized constructor that sets the message and the error code
        /// </summary>
        /// <param name="message">message to be used in the exception handling</param>
        /// <param name="code">error code to identify the exception</param>
        /// <returns>New ElectionGuardException object</returns>
        public ElectionGuardException(string message, Status code) : base(message)
        {
            this.Status = code;
        }

        /// <summary>
        /// Parameterized constructor that uses another exception and wraps it
        /// </summary>
        /// <param name="message">message to be used in the exception handling</param>
        /// <param name="inner">exception to wrap inside the ElectionGuardException</param>
        /// <returns>New ElectionGuardException object</returns>
        public ElectionGuardException(string message, System.Exception inner) : base(message, inner) { }
    }

}