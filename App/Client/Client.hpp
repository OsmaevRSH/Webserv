#pragma once

#include "Parse_input_handler.hpp"

class Client
{
	public:
		Parse_input_handler*	_client_handler;
		std::string				_request_header;
		std::string				_request_body;
		std::string				_ready_response_to_the_customer;
		std::string				_server_client_ip;
		std::string				_chunked_end_check;
		bool					_chunked_end_check_status;
		int						_content_length_buffer;
		int						_chunked_length;
		bool					_chunked_length_status;
		int						_client_fd;
		int						_curent_progress; //0-nothing, 1-handler, 2-body, 3-done
		bool					_answer_is_ready;

		Client()
		:	_client_handler(nullptr),
			_content_length_buffer(0),
			_chunked_length(0),
			_chunked_length_status(false),
			_client_fd(0),
			_curent_progress(0),
			_answer_is_ready(false),
			_chunked_end_check_status(false) {}
};

