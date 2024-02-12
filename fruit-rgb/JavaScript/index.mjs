import express from "express";
import mysql from 'mysql';

const app = express(); // define express server
app.use( (req,res, next) => {
    res.append('Access-Control-Allow-Origin', ['*']);
    res.append('Access-Control-Allow-Headers', ['*']);
    res.append('Access-Control-Allow-Methods', 'GET,POST');
    next();
});

app.use(express.json());

const db = mysql.createConnection({ // connect to MySQL DB
     user : 'app_usr',
     password : '',
     host : 'localhost',
     database : 'sys'
 });

db.connect();

app.listen(3000, () => // begin listen
  console.log('Example app listening on port 3000!'),
);

app.get('/data', (req,res) => { // return most recent DB SensorGrades table entry
    const result = db.query('SELECT * FROM SensorGrades ORDER BY idSensorGrades DESC LIMIT 1', (error,results,fields)=>{
        if (error) throw error;
        res.send(results)
    });
})

app.post('/id?', async (req,res) => {
    const parameters = {idvalue : req.body.id}
    const result = db.query('SELECT * FROM SensorGrades WHERE idSensorGrades = ?', parameters.idvalue ,(error, results, fields) => {
        if(error) throw error;
        res.send(results)
    });
});

