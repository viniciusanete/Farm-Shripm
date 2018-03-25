package br.com.unigranrio.xavante.controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class HomeController {
	
	@RequestMapping(value="/")
	public String home() {
		return "Helllo World";
	}
	
	
	@RequestMapping(value="/private")
	public String privado() {
		return "Hello private";
	}
	
	
}
