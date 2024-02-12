import SwiftUI

struct ContentView: View {
    var body: some View {
        NavigationView {
            VStack (spacing: 20) {
                
                Text("🔊🔍")
                    .font(.title)
                
                Text("Welcome to AudioFinder!")
                    .font(.title)
                
                Spacer()
                    .frame(height: 1)
                
                Text("Before you jump in...")
                    .font(.headline)
                
                VStack (alignment: .leading, spacing: 10) {
                    
                    Text("As a beginner coder, most of my time is spent following and reviewing video tutorials on certain coding languages.")
                        
                    
                    Text("A recurring issue, however, was when I wanted to review a concept or method mentioned in the tutorial but only found myself skipping back and forth through the video, spending too much time on the search.")
                    
                    Text("I would always think to myself: \"I wish there were some type of Ctrl+F (or Command+F) for audio!\"")
                    
                    Text("AudioFinder is a solution to this problem, using Apple's Speech-To-Text technology to recognize words in an audio file and determine its position in time.")
                    
                } // VStack
                .padding(5)
                
                NavigationLink(destination: AudioFinderView()) {
                    Text("Get Started")
                } // NavigationLink
                
                
                Spacer()
                
            } // VStack
            .padding(40)
            
        } // NavigationView
    } // View
} // ContentView

struct ContentView_Preview: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
